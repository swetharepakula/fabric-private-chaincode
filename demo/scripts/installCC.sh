#!/bin/bash

#
# Copyright IBM Corp All Rights Reserved
#
# SPDX-License-Identifier: Apache-2.0
#

set -ev

export WAIT_TIME=15
export version=1.0
export FABRIC_BIN_DIR=/project/src/github.com/hyperledger/fabric/.build/bin
export PEER_CMD=/project/src/github.com/hyperledger-labs/fabric-private-chaincode/fabric/bin/peer.sh
export FPC_DIR=/project/src/github.com/hyperledger-labs/fabric-private-chaincode

docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" -e "CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/msp/users/Admin@org1.example.com/msp" peer0.org1.example.com ${PEER_CMD} chaincode install -n mockcc -v ${version} --path github.com/hyperledger-labs/fabric-private-chaincode/demo/chaincode/golang/cmd -l golang
sleep ${WAIT_TIME}

docker exec -e "CORE_PEER_LOCALMSPID=Org1MSP" -e "CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/msp/users/Admin@org1.example.com/msp" -e "ORDERER_ADDR=orderer.example.com:7050" peer0.org1.example.com ${PEER_CMD} chaincode instantiate -n mockcc -v ${version} --channelID mychannel -o orderer.example.com:7050 -l golang -c '{"Args":[]}'
sleep ${WAIT_TIME}
