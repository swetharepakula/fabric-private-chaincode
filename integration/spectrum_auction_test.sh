#!/bin/bash

# Copyright Intel Corp. All Rights Reserved.
#
# SPDX-License-Identifier: Apache-2.0

SCRIPTDIR="$(dirname $(readlink --canonicalize ${BASH_SOURCE}))"
FPC_TOP_DIR="${SCRIPTDIR}/.."
FABRIC_SCRIPTDIR="${FPC_TOP_DIR}/fabric/bin/"

: ${FABRIC_CFG_PATH:="${SCRIPTDIR}/config"}

. ${FABRIC_SCRIPTDIR}/lib/common_utils.sh
. ${FABRIC_SCRIPTDIR}/lib/common_ledger.sh

CC_ID=spectrum_auction_test

#this is the path that will be used for the docker build of the chaincode enclave
ENCLAVE_SO_PATH=demo/clock-auction-chaincode/_build/lib/

CC_VERS=0
FAILURES=0
set -x
spectrum_auction_test() {
    # install, init, and register (auction) chaincode
    try ${PEER_CMD} chaincode install -l fpc-c -n ${CC_ID} -v ${CC_VERS} -p ${ENCLAVE_SO_PATH}
    sleep 3

    try ${PEER_CMD} chaincode instantiate -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -v ${CC_VERS} -c '{"Args":[]}' -V ecc-vscc
    sleep 3

    # run api
    echo "Running API (5s)..."
    sleep 5
    CC_CMD_STRING="{\"Function\":\"createAuction\",\"Args\":[\"nothing\"]}"
    echo ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c ${CC_CMD_STRING} --waitForEvent
    ${PEER_CMD} chaincode invoke -o ${ORDERER_ADDR} -C ${CHAN_ID} -n ${CC_ID} -c ${CC_CMD_STRING} --waitForEvent
    echo "API returned"
    sleep 5
}

# 1. prepare
para
say "Preparing Test ..."
# - clean up relevant docker images
docker_clean ${ERCC_ID}
docker_clean ${CC_ID}

trap ledger_shutdown EXIT

para

say "- setup ledger"
ledger_init

say "- test"
spectrum_auction_test

say "- shutdown ledger"
ledger_shutdown

para
exit 0

