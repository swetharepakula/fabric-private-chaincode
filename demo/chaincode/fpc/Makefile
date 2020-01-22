# Copyright 2019 Intel Corporation
# Copyright IBM Corp. All Rights Reserved.
#
# SPDX-License-Identifier: Apache-2.0

TOP = $(FPC_PATH)
include $(TOP)/build.mk

BUILD_DIR := _build

$(BUILD_DIR):
	@if [ ! -d $(BUILD_DIR) ]; then \
		mkdir -p $(BUILD_DIR) && \
		cd $(BUILD_DIR) && \
		cmake ./..; \
	fi

build: $(BUILD_DIR)
	$(MAKE) --directory=$<

clean:
	-rm -rf $(BUILD_DIR)

docker-build: clean
	$(DOCKER) run -v ${PWD}:/project/src/github.com/hyperledger-labs/fabric-private-chaincode/demo/chaincode hyperledger/fabric-private-chaincode-cc-builder sh -c make build
