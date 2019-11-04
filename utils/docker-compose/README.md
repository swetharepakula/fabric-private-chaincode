# FPC Docker Compose Network
This docker-compose example has been adapted from a [Fabric 101 Workshop](https://github.com/swetharepakula/Fabric101Workshop) which was adapted from the basic
network and fabcar example in the [Fabric Samples](https://github.com/hyperledger/fabric-samples).
This example does not use TLS, which means the Fabric Go SDK cannot be
used to interact with the network. Currently, there is two orgs, one peer,
one orderer, and one fabric-ca in the network.

## Configuration
- [core.yaml](core.yaml) : Peer configuration that has the SGX plugins and
locations relative to location within docker image
- [orderer.yaml](orderer.yaml) : Orderer configuration
- [crypto-config.yaml](crypto-config.yaml) : File used with cryptogen to generate
certs for specified number of orgs, peers, users, and orderer. Output CAs can be
used to start instances of fabric-ca
- [configtx.yaml](configtx.yaml)  : File used with configtxgen to generate the
genesis block which is used as the basis of the specified channel
- [docker-compose.yml](docker-compose.yml) : Configuration of the fabric network
to be used with `docker-compose`. **Docker version 17.06.2-ce is needed**

## Steps
1. Download the necessary fabric binaries. Run the
[bootstrap script](scripts/bootstrap.sh) which will download the Fabric 1.4.3
into a local bin directory. If you already have the binaries downloaded in your
path, this step can be skipped. **Fabric 1.4.3 versions of configtxgen and
cryptogen are required to use the configurations above.**
```
./scripts/bootstrap.sh
```
1. Build the peer image by running the following in the root of this repository.
The [Dockerfile](../Dockerfile) is in the root of the repository. This step
assumes you have already build the [fabric-private-chaincode base image](../utils/docker/base/Dockerfile).
This will create the modified peer image as well as make the plugins necessary
to start the peer.
```
docker build -t hyperledger/fabric-peer-fpc .
```
1. Generate the cryptographic material needed for the network by running the
[generate](generate.sh) script. Cryptogen will be used to generate all the
credentials needed based on the configuration filesabove and place them in the
`crypto-config` directory.  Configtxgen will be used to create the genesis block
which is used to start up the orderer as well as the peer create channel
configuration transaction. These will be placed in the `config` directory. The
`crypto-config` & `config` directory will be mounted into every container of the
FPC network as specified in the docker-compose file. **This script is not
idempotent and will delete the contents of `crypto-config` & `config` when run
to ensure a clean start.**
```
./generate.sh
```
1. Start the network. Run the [start](start.sh) script. This will use
docker-compose to start the network as well as starting the channel `mychannel`.
```
./start.sh
```

## Deploying you FPC Chaincode
The examples directory has been mounted into the peer container for convenience,
under `/opt/examples`. All of these steps should be done within the peer container.

1. Exec into the peer container.
```
docker exec -it peer0.org1.example.com bash
```

1. Follow the [steps](../../examples/README.md) in the tutorial to build your
chaincode within the peer container. Do not continue to the testing step.

1. Set up the peer command environment variable for convenience. `$FPC_CMDS` is
already defined in the container.
```
export PEER_CMD=$FPC_CMDS/peer.sh
```

1. Set environment variable to use the admin credentials and set the
orderer address.
```
export CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/msp/users/Admin@org1.example.com/msp
export ORDERER_ADDR=orderer.example.com:7050
```

1. Install your chaincode
```
${PEER_CMD} chaincode install -l fpc-c -n helloworld_test -v 0 -p examples/helloworld/_build/lib
```

1. Instantiate your chaincode
```
${PEER_CMD} chaincode instantiate -o orderer.example.com:7050 -C mychannel -n helloworld_test -v 0 -c '{"Args":["init"]}' -V ecc-vscc
```

## Interact with the FPC Chaincode
1. Store asset1 with a value of a 100
```
${PEER_CMD} chaincode invoke -C mychannel -n helloworld_test -c '{"Args":["storeAsset","asset1","100"]}'
```

1. Retrieve the current value of asset1.
```
${PEER_CMD} chaincode query -C mychannel -n helloworld_test -c '{"Args":["retrieveAsset","asset1"]}'
```
The response should look like the following:
```
{
    "ResponseData":"YXNzZXQxOjEwMA==",
    "Signature":<signature>,
    "PublicKey": <public-key>
}
```

1. Verify the encrypted response data shows that asset1 is equal to a hundred.
```
> echo "YXNzZXQxOjEwMA==" | base64 -d
asset1:100
```


## Teardown the network

1. Run the [teardown script](./teardown.sh) to clean up your environment. Run this in the root of this repo. **NOTE** This will try to
remove all your containers and prune all excess volumes.
```
./teardown.sh
```

## Places for Improvements
- The current FPC dev image + peer is quite large
- Making the plugin build simpler. One step I have already done is making a
separate make target for the plugins.
