#!/bin/bash
#
# Copyright IBM Corp All Rights Reserved
#
# SPDX-License-Identifier: Apache-2.0
#
# Exit on first error, print all commands.
set -ev

export SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

# don't rewrite paths for Windows Git Bash users
export MSYS_NO_PATHCONV=1
export COMPOSE_PROJECT_NAME=fabric101
export NETWORK_CONFIG=${SCRIPT_DIR}/../network-config
if [[ $USE_FPC = false ]]; then
    export PEER_CMD=peer
else
    export FPC_CONFIG=-fpc
    export PEER_CMD=/project/src/github.com/hyperledger-labs/fabric-private-chaincode/fabric/bin/peer.sh
    # FABRIC_BIN_DIR needs to be set for FPC Peer CMD
    export FABRIC_BIN_DIR=/project/src/github.com/hyperledger/fabric/.build/bin
fi

docker-compose -f ${NETWORK_CONFIG}/docker-compose.yml down

docker-compose -f ${NETWORK_CONFIG}/docker-compose.yml up -d orderer.example.com peer0.org1.example.com ca.example.com
docker ps -a

# wait for Hyperledger Fabric to start
# incase of errors when running later commands, issue export FABRIC_START_TIMEOUT=<larger number>
export FABRIC_START_TIMEOUT=10
sleep ${FABRIC_START_TIMEOUT}

# Create the channel
docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" -e "CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/msp/users/Admin@org1.example.com/msp" peer0.org1.example.com ${PEER_CMD} channel create -o orderer.example.com:7050 -c mychannel -f /etc/hyperledger/configtx/channel.tx
# Join peer0.org1.example.com to the channel.
docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" -e "CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/msp/users/Admin@org1.example.com/msp" peer0.org1.example.com ${PEER_CMD} channel join -b mychannel.block
