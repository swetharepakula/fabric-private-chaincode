#!/bin/bash

#
# Copyright IBM Corp All Rights Reserved
#
# SPDX-License-Identifier: Apache-2.0
#

export DEMO_SCRIPTS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
export FPC_ROOT=${DEMO_SCRIPTS_DIR}/../..
export DEMO_ROOT=${DEMO_SCRIPTS_DIR}/..

# SCRIPT_DIR is the docker compose script dir that needs to be defined to source environment variables from the FPC Network
export SCRIPT_DIR=${FPC_ROOT}/utils/docker-compose/scripts
. ${SCRIPT_DIR}/lib/common.sh

# Cleanup any previous iterations of the demo
"${SCRIPT_DIR}/teardown.sh" --clean-slate

# Generate the necessary credentials and start the FPC network
"${SCRIPT_DIR}/generate.sh"
"${SCRIPT_DIR}/start.sh"

# Install and Instanciate Auction Chaincode
"${DEMO_SCRIPTS_DIR}/installCC.sh"

# Register Users
"${DEMO_ROOT}/client/backend/fabric-gateway/registerUsers.sh"

# Build Auction Client Backend
pushd  "${DEMO_ROOT}/client/backend/fabric-gateway"
  docker build -t auction_client_backend -f "${DEMO_ROOT}/client/backend/fabric-gateway/Dockerfile" .
popd

# Build Auction Client Frontend
pushd  "${DEMO_ROOT}/client/frontend"
  make build
popd


# Run the Auction Client, the COMPOSE_PROJECT_NAME is set in ${SCRIPT_DIR}/
COMPOSE_IGNORE_ORPHANS=true ${DOCKER_COMPOSE_CMD} -f ${DEMO_ROOT}/docker-compose.yml up -d auction_client auction_frontend

