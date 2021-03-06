#!/bin/bash
# Copyright Intel Corp. All Rights Reserved.
#
# SPDX-License-Identifier: Apache-2.0

#RUN=echo # uncomment to dry-run peer call

SCRIPTDIR="$(dirname $(readlink --canonicalize ${BASH_SOURCE}))"
FPC_TOP_DIR="${SCRIPTDIR}/../../"
FABRIC_SCRIPTDIR="${FPC_TOP_DIR}/fabric/bin/"

: ${FABRIC_CFG_PATH:=$(pwd)}

. ${FABRIC_SCRIPTDIR}/lib/common_utils.sh
. ${FABRIC_SCRIPTDIR}/lib/common_ledger.sh

ARGS_EXEC=( "$@" ) # params to eventually pass to real orderer /default: just pass all original args ..


# Call real orderer
try $RUN exec ${FABRIC_BIN_DIR}/configtxgen "${ARGS_EXEC[@]}"
