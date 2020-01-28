#!/bin/bash

#
# Copyright IBM Corp All Rights Reserved
#
# SPDX-License-Identifier: Apache-2.0
#

set -e

export DEMO_SCRIPTS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
export FPC_ROOT=${DEMO_SCRIPTS_DIR}/../..
export DEMO_ROOT=${DEMO_SCRIPTS_DIR}/..

# SCRIPT_DIR is the docker compose script dir that needs to be defined to source environment variables from the FPC Network
export SCRIPT_DIR=${FPC_ROOT}/utils/docker-compose/scripts
. ${SCRIPT_DIR}/lib/common.sh


help(){
   echo "$(basename $0) [options]

   This script, by default, will teardown possible previous iterations of this demo, generate new
   crypto material for the network, start an FPC network as defined in \$FPC_PATH/utils/docker-compose,
   install the mock golang auction chaincode(\$FPC_PATH/demo/chaincode/golang), install the FPC
   compliant auction chaincode(\$FPC_PATH/demo/chaincode/fpc), register auction users, and bring up
   both the fabric-gatway & frontend UI. If the fabric-gateway and frontend UI docker images have
   not previously been built it will build them, otherwise the script will reuse the images already
   existing. The FPC chaincode will not be built unless specified by the flag --build-cc.

   options:
       --build-cc:
           As part of bringing up the demo components, the auction cc in demo/chaincode/fpc will
           be rebuilt using the docker-build make target. The image
           hyperledger/fabric-private-chaincode-cc-builder must exist to use this option.
       --build-client:
           As part of bringing up the demo components, the Fabric Gateway and the UI docker images 
           will be built or rebuilt using current source code.
       --help,-h:
           Print this help screen.
    "
}


BUILD_CHAINCODE=false
BUILD_CLIENT=false
for var in "$@"; do
    case "$var" in
        "--build-cc")
            BUILD_CHAINCODE=true
            ;;
        "--build-client")
            BUILD_CLIENT=true
            ;;
        "-h"|"--help")
            help
            exit
            ;;
        *)
            echo "Invalid option passed: ${var}"
            help
            exit
            ;;
    esac
    shift
done

# Cleanup any previous iterations of the demo
"${SCRIPT_DIR}/teardown.sh"

# Generate the necessary credentials and start the FPC network
"${SCRIPT_DIR}/generate.sh"

if [ $BUILD_CHAINCODE ]; then
    echo "Building FPC Auction Chaincode"
    echo "\$BUILD_CHAINCODE=${BUILD_CHAINCODE}"

    # Build FPC Chaincode
    pushd ${DEMO_ROOT}/chaincode/fpc
        FPC_PATH=${FPC_ROOT} make docker-build
    popd
fi

if [ $BUILD_CLIENT ]; then
    echo "Building Fabric Gateway and Frontend UI"
    # Build Frontend, & Gateway
    COMPOSE_IGNORE_ORPHANS=true ${DOCKER_COMPOSE_CMD} -f ${DEMO_ROOT}/docker-compose.yml build
fi

# Start the FPC Network using utils/docker-compose scripts
echo "Starting the FPC Network"
COMPOSE_IGNORE_ORPHANS=true "${SCRIPT_DIR}/start.sh"

# Install and Instantiate Auction Chaincode
echo "Installing mockcc and auctioncc"
"${DEMO_SCRIPTS_DIR}/installCC.sh"

# Register Users
echo "Registering Auction Users with Fabric CA"
"${DEMO_ROOT}/client/backend/fabric-gateway/registerUsers.sh"

echo "Bringing up the Fabric Gateway and Frontend UI"
# Run the Auction Client, the COMPOSE_PROJECT_NAME is set in ${SCRIPT_DIR}/
# Since the new containers are going to use the same network as the FPC network, docker-compose
# typically throws a warning as it sees containers using the network. To quiet the warning set
# COMPOSE_IGNORE_ORPHANS to true.
COMPOSE_IGNORE_ORPHANS=true ${DOCKER_COMPOSE_CMD} -f ${DEMO_ROOT}/docker-compose.yml up -d auction_client auction_frontend
