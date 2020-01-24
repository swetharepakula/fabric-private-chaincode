#!/bin/bash
#
# Copyright IBM Corp All Rights Reserved
#
# SPDX-License-Identifier: Apache-2.0
#

set -e

DEMO_SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
DEMO_DOCKER_COMPOSE=${DEMO_SCRIPT_DIR}/../docker-compose.yml

# SCRIPT_DIR is the docker compose script dir that needs to be defined to source environment variables from the FPC Network
SCRIPT_DIR=${DEMO_SCRIPT_DIR}/../../utils/docker-compose/scripts
. ${SCRIPT_DIR}/lib/common.sh

help(){
   echo "$(basename $0) [options]

   This script, by default, will teardown all of the auction demo components, but will not prune
   volumes or delete the chaincode images that have been created for the mockcc and the auctioncc

   options:
       --clean-slate:
           As part of the teardown, prune all unused docker volumes and delete mockcc and auctioncc
           images.
       --help,-h:
           Print this help screen.
       --network-only,-n:
           The script will not teardown the fabric-gateway or the UI. It will only teardown the
           FPC Network as definied in \$FPC_PATH/utils/docker-compose.
    "
}

CLEAN_SLATE=false
NETWORK_ONLY=false
for var in "$@"; do
    case "$var" in
        "--clean-slate")
            BUILD_CHAINCODE=true
            ;;
        "-n"|"--network-only")
            NETWORK_ONLY=true
            ;;
        "-h"|"--help"|*)
            help
            exit
            ;;
    esac
    shift
done

if [ ! $NETWORK_ONLY ]; then
    COMPOSE_IGNORE_ORPHANS=true docker-compose -f ${DEMO_DOCKER_COMPOSE} kill && COMPOSE_IGNORE_ORPHANS=true docker-compose -f ${DEMO_DOCKER_COMPOSE} down
fi


if [ $CLEAN_SLATE ]; then
    "${SCRIPT_DIR}/teardown.sh" --clean-slate
    images=$(docker images dev_*mockcc-* -q)
    if [ ! -z "${images}" ]; then
        docker rmi "${images}"
    fi

    images=$(docker images dev_*auctioncc-* -q)
    if [ ! -z "${images}" ]; then
        docker rmi "${images}"
    fi
    exit
fi

"${SCRIPT_DIR}/teardown.sh"
