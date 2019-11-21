# Copyright IBM Corp. All Rights Reserved.
#
# SPDX-License-Identifier: Apache-2.0

GOFLAGS :=
GO := go $(GOFLAGS)

DOCKERFLAGS :=
DOCKER := docker $(DOCKERFLAGS)

PROJECT_NAME=fabric-private-chaincode

export SGX_MODE ?= HW
export SGX_BUILD ?= PRERELEASE

export FABRIC_PATH ?= ${GOPATH}/src/github.com/hyperledger/fabric

JAVA ?= java
PLANTUML_JAR ?= plantuml.jar
PLANTUML_CMD ?= $(JAVA) -jar $(PLANTUML_JAR)
PLANTUML_IMG_FORMAT ?= png # pdf / png / svg
DOCKER_DEV_RUN_OPTS= -v /Users/Repakula/go/src/github.com/hyperledger-labs/fabric-private-chaincode:/project/src/github.com/hyperledger-labs/fabric-private-chaincode
