# Copyright IBM Corp. All Rights Reserved.
#
# SPDX-License-Identifier: Apache-2.0

FROM hyperledger/fabric-private-chaincode-dev:latest

# RUN cd $FPC_PATH
# RUN make min

RUN cd $GOPATH
RUN rm -rf $FPC_PATH
RUN mkdir -p $FPC_PATH

RUN mkdir /tmp/build-src
COPY . /tmp/build-src

ARG FPC_REPO_URL=https://github.com/hyperledger-labs/fabric-private-chaincode.git
ARG FPC_REPO_BRANCH=master

# RUN echo $FPC_REPO_URL
# RUN echo $FPC_REPO_BRANCH

RUN git clone --branch $FPC_REPO_BRANCH $FPC_REPO_URL $FPC_PATH

RUN cd $FPC_PATH
RUN make min

WORKDIR $FPC_PATH/demo/chaincode
