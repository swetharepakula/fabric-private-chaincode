/*
Copyright IBM Corp. All Rights Reserved.

SPDX-License-Identifier: Apache-2.0

TODO (eventually):
  eventually refactor the mock backend as it has the potential of wider
  usefulness (the same applies also to the fabric gateway).
  Auction-specific aspects;
   - the bridge change-code has auction in names (trivial to remove)
   - the "/api/getRegisteredUsers" and, in particular,
     "/api/clock_auction/getDefaultAuction", are auction-specific
   - processing of response

  PS: probably also worth moving the calls to __init & __setup as well
  as the unpacking of the payload objects, which are specific to FPC
  to chaincode/fpc_chaincode.go (or handle these calls for non-fpc
  in chaincode/go_chaincode.go such that actual go chaincode doesn't
   have to know about it?)
*/

package main

import (
	"encoding/json"
	"flag"
	"fmt"
	"net/http"

	"github.com/gin-contrib/cors"
	"github.com/gin-gonic/gin"
	"github.com/hyperledger-labs/fabric-private-chaincode/demo/client/backend/mock/api"
	"github.com/hyperledger-labs/fabric-private-chaincode/demo/client/backend/mock/chaincode"
	"github.com/hyperledger-labs/fabric-private-chaincode/utils"
	"github.com/hyperledger/fabric/core/chaincode/shim"
)

var flagPort string
var flagDebug bool
var stub *shim.MockStub
var logger = shim.NewLogger("server")

const ccName = "ecc"

func init() {
	flag.StringVar(&flagPort, "port", "3000", "Port to listen on")
	flag.BoolVar(&flagDebug, "debug", false, "debug output")
}

func main() {
	flag.Parse()

	if flagDebug {
		logger.SetLevel(shim.LogDebug)
	}

	stub = shim.NewMockStub(ccName, chaincode.NewMockAuction())

	// setup and init
	stub.MockInvoke("someTxID", [][]byte{[]byte("__setup"), []byte("ercc"), []byte("mychannel"), []byte("tlcc")})
	stub.MockInvoke("1", [][]byte{[]byte("__init"), []byte("My Auction")})

	config := cors.DefaultConfig()
	config.AllowAllOrigins = true
	config.AllowHeaders = append(config.AllowHeaders, "x-user")

	r := gin.Default()
	r.Use(cors.New(config))

	r.GET("/api/getRegisteredUsers", getAllUsers)
	r.GET("/api/clock_auction/getDefaultAuction", getDefaultAuction)
	r.POST("/api/cc/invoke", invoke)
	// note that using a MockStub there is no need to differentiate between query and invoke
	r.POST("/api/cc/query", invoke)

	r.Run(":" + flagPort)
}

func getAllUsers(c *gin.Context) {
	users := api.MockData["getRegisteredUsers"]
	c.IndentedJSON(http.StatusOK, users)
}

func getDefaultAuction(c *gin.Context) {
	users := api.MockData["getDefaultAuction"]
	c.IndentedJSON(http.StatusOK, users)
}

type Payload struct {
	Tx   string
	Args []string
}

func invoke(c *gin.Context) {

	user := c.GetHeader("x-user")
	logger.Debug(fmt.Sprintf("user: %s\n", user))

	// TODO set creator

	args, err := parsePayload(c)
	if err != nil {
		//fmt.Println("Request Error: " + err.Error())
		logger.Error(fmt.Sprintf("Request Error: %s\n", err))
		c.AbortWithStatusJSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}

	res := stub.MockInvoke("someTxID", args)
	logger.Debugf("invocation response: status='%v' / payload='%v' / message='%s'", res.Status, res.Payload, res.Message)

	// NOTE: we (try to) return error even if the invocation get success back
	// but does not contain a response payload. According to the auction
	// specifications, all queries and transactions should return a response
	// object (even more specifically, an object which at the very least
	// contains a 'status' field)
	var fpcResponse string
	// we might get payload and response regardless of invocation success,
	// so try to decode in all cases
	if res.Payload != nil {
		var response utils.Response
		// unwarp ecc response and return only responseData
		// a proper client would now also verify response signature
		if err = json.Unmarshal(res.Payload, &response); err == nil {
			logger.Debugf("decoded fpc response: ResponseData='%s'",
				response.ResponseData)
			fpcResponse = string(response.ResponseData)
		}
		if err != nil {
			fpcResponse = fmt.Sprintf("{ \"status\": { \"rc\" : %d, \"message\" : \"No valid response payload received due to errror=%v (status=%v/message=%v)\" } }",
				499, err, res.Status, res.Message) // TODO: better/smarter errors ..
		}
		// Note: we do _not_ check whether fpcResponse is a valid
		// status object, just that it's not empty
		if fpcResponse == "" {
			fpcResponse = fmt.Sprintf("{ \"status\": { \"rc\" : %d, \"message\" : \"Invalid empty response payload received (status=%v/message=%v)\" } }",
				499, res.Status, res.Message) // TODO: better/smarter errors ..
		}
		// TODO (eventually): check also whether it is a proper response,
		// i.e., a object which contains at least a field 'status' which
		// itself is an object with a rc and message field ...
	} else {
		fpcResponse = fmt.Sprintf("{ \"status\": { \"rc\" : %d, \"message\" : \"No response payload received (status=%v/message=%v)\" } }",
			499, res.Status, res.Message) // TODO: better/smarter errors ..
	}

	c.Data(http.StatusOK, c.ContentType(), []byte(fpcResponse))
}

func parsePayload(c *gin.Context) ([][]byte, error) {
	var payload Payload
	if err := c.ShouldBindJSON(&payload); err != nil {
		return nil, err
	}

	args := make([][]byte, 0)
	args = append(args, []byte(payload.Tx))
	for _, b := range payload.Args {
		args = append(args, []byte(b))
	}

	return args, nil
}
