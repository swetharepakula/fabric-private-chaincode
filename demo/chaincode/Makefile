# Copyright 2019 Intel Corporation
# Copyright IBM Corp. All Rights Reserved.
#
# SPDX-License-Identifier: Apache-2.0

TOP = ../..
include $(TOP)/build.mk
FPC_DOCKER_NAMESPACE := hyperledger/fabric-private-chaincode
FPC_PEER_DOCKER_NAMESPACE := hyperledger/fabric-peer-fpc

DOCKER_DAEMON_SOCKET ?= /var/run/docker.sock
FABRIC_PEER_DAEMON_CHAINCODE_PORT ?= 7052

DOCKER_DEV_BUILD_OPTS ?=
ifdef DOCKER_DEV_IMAGE_APT_ADD__PKGS
	DOCKER_DEV_BUILD_OPTS += --build-arg APT_ADD_PKGS=$(DOCKER_DEV_IMAGE_APT_ADD__PKGS)
endif


DOCKER_DEV_RUN_OPTS ?=
DOCKER_DEV_RUN_OPTS += -p $(FABRIC_PEER_DAEMON_CHAINCODE_PORT):$(FABRIC_PEER_DAEMON_CHAINCODE_PORT) -v $(DOCKER_DAEMON_SOCKET):$(DOCKER_DAEMON_SOCKET)
OCKER_GOPATH=/project
DOKER_FPCPATH=$(DOCKER_GOPATH)/src/github.com/hyperledger-labs/fabric-private-chaincode
SGX_DEVICE_PATH ?= $(shell if [ -e "/dev/isgx" ]; then echo "/dev/isgx"; elif [ -e "/dev/sgx" ]; then echo "/dev/sgx"; fi)
SGX_PSW_SOCKET ?= /var/run/aesmd
ifneq ($(SGX_DEVICE_PATH),)
	DOCKER_DEV_RUN_OPTS += -v $(PWD)/../../config/ias/:$(DOKER_FPCPATH)/config/ias/ -v $(SGX_PSW_SOCKET):$(SGX_PSW_SOCKET) --device $(SGX_DEVICE_PATH)
endif

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

# docker-build:
# 	docker run -v ${PWD}:/project/src/github.com/hyperledger-labs/fabric-private-chaincode/demo/chaincode -v /var/run/docker.sock:/var/run/docker.sock hyperledger/fabric-private-chaincode-dev sh -c make && cd $FPC_PATH/demo/chaincode && make clean && make

docker-build:
	$(DOCKER) run -v ${PWD}:/project/src/github.com/hyperledger-labs/fabric-private-chaincode/demo/chaincode hyperledger/fabric-private-chaincode-builder sh -c make build
