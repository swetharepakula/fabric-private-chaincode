# FPC Docker Compose Network
This docker-compose example has been adapted from a [Fabric 101 Workshop](https://github.com/swetharepakula/Fabric101Workshop) which was adapted from the basic
network and fabcar example in the [Fabric Samples](https://github.com/hyperledger/fabric-samples). This example does not use TLS, which means the Fabric Go SDK cannot be
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
path, this step can be skipped. **Fabric 1.4.3. versions of configtxgen and
cryptogen are required to use the configurations above.**
```
./scripts/bootstrap.sh
```
1. Build the peer image by running the following in the root of this repository.
The [Dockerfile](../Dockerfile) is in the root of the repository. This step
assumes you have already build the (fabric-private-chaincode base image)[../utils/docker/base/Dockerfile]. This will create the modified peer image as well as make the
plugins necessary to start the peer.
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
idempotent and will delete the contetnts of `crypto-config` & `config` when run
to ensure a clean start.**
```
./generate.sh
```
1. Start the network. Run the [start](start.sh) script. This will use
docker-compose to start the network as well as starting the channel `mychannel`.
```
./start.sh
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
