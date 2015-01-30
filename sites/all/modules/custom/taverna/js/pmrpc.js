/*
 * pmrpc 0.7.1 - Inter-widget remote procedure call library based on HTML5
 *               postMessage API and JSON-RPC. https://github.com/izuzak/pmrpc
 *
 * Copyright 2012 Ivan Zuzak, Marko Ivankovic
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

pmrpc = self.pmrpc =  function() {
  // check if JSON library is available
  if (typeof JSON === "undefined" || typeof JSON.stringify === "undefined" ||
      typeof JSON.parse === "undefined") {
    throw "pmrpc requires the JSON library";
  }

  // TODO: make "contextType" private variable
  // check if postMessage APIs are available
  if (typeof this.postMessage === "undefined" &&  // window or worker
        typeof this.onconnect === "undefined") {  // shared worker
      throw "pmrpc requires the HTML5 cross-document messaging and worker APIs";
  }

  // Generates a version 4 UUID
  function generateUUID() {
    var uuid = [], nineteen = "89AB", hex = "0123456789ABCDEF";
    for (var i=0; i<36; i++) {
      uuid[i] = hex[Math.floor(Math.random() * 16)];
    }
    uuid[14] = '4';
    uuid[19] = nineteen[Math.floor(Math.random() * 4)];
    uuid[8] = uuid[13] = uuid[18] = uuid[23] = '-';
    return uuid.join('');
  }

  // Checks whether a domain satisfies the access control list. The access
  // control list has a whitelist and a blacklist. In order to satisfy the acl,
  // the domain must be on the whitelist, and must not be on the blacklist.
  function checkACL(accessControlList, origin) {
    var aclWhitelist = accessControlList.whitelist;
    var aclBlacklist = accessControlList.blacklist;

    var isWhitelisted = false;
    var isBlacklisted = false;

    for (var i=0; i<aclWhitelist.length; ++i) {
      if(origin.match(new RegExp(aclWhitelist[i]))) {
        isWhitelisted = true;
        break;
      }
    }

    for (var j=0; j<aclBlacklist.length; ++j) {
      if(origin.match(new RegExp(aclBlacklist[j]))) {
        isBlacklisted = true;
        break;
      }
    }

    return isWhitelisted && !isBlacklisted;
  }

  // Calls a function with either positional or named parameters
  // In either case, additionalParams will be appended to the end
  function invokeProcedure(fn, self, params, additionalParams) {
    if (!(params instanceof Array)) {
      // get string representation of function
      var fnDef = fn.toString();

      // parse the string representation and retrieve order of parameters
      var argNames = fnDef.substring(fnDef.indexOf("(")+1, fnDef.indexOf(")"));
      argNames = (argNames === "") ? [] : argNames.split(", ");

      var argIndexes = {};
      for (var i=0; i<argNames.length; i++) {
        argIndexes[argNames[i]] = i;
      }

      // construct an array of arguments from a dictionary
      var callParameters = [];
      for (var paramName in params) {
        if (typeof argIndexes[paramName] !== "undefined") {
          callParameters[argIndexes[paramName]] = params[paramName];
        } else {
          throw "No such param: " + paramName;
        }
      }

      params = callParameters;
    }

    // append additional parameters
    if (typeof additionalParams !== "undefined") {
      params = params.concat(additionalParams);
    }

    // invoke function with specified context and arguments array
    return fn.apply(self, params);
  }

  // JSON encode an object into pmrpc message
  function encode(obj) {
    return "pmrpc." + JSON.stringify(obj);
  }

  // JSON decode a pmrpc message
  function decode(str) {
    return JSON.parse(str.substring("pmrpc.".length));
  }

  // Creates a base JSON-RPC object, usable for both request and response.
  // As of JSON-RPC 2.0 it only contains one field "jsonrpc" with value "2.0"
  function createJSONRpcBaseObject() {
    var call = {};
    call.jsonrpc = "2.0";
    return call;
  }

  // Creates a JSON-RPC request object for the given method and parameters
  function createJSONRpcRequestObject(procedureName, parameters, id) {
    var call = createJSONRpcBaseObject();
    call.method = procedureName;
    call.params = parameters;
    if (typeof id !== "undefined") {
      call.id = id;
    }
    return call;
  }

  // Creates a JSON-RPC error object complete with message and error code
  function createJSONRpcErrorObject(errorcode, message, data) {
    var error = {};
    error.code = errorcode;
    error.message = message;
    error.data = data;
    return error;
  }

  // Creates a JSON-RPC response object.
  function createJSONRpcResponseObject(error, result, id) {
    var response = createJSONRpcBaseObject();
    response.id = id;

    if (typeof error === "undefined" || error === null) {
      response.result = (result === "undefined") ? null : result;
    } else {
      response.error = error;
    }

    return response;
  }

  // dictionary of services registered for remote calls
  var registeredServices = {};
  // dictionary of requests being processed on the client side
  var callQueue = {};

  var reservedProcedureNames = {};
  // register a service available for remote calls
  // if no acl is given, assume that it is available to everyone
  function register(config) {
    if (config.publicProcedureName in reservedProcedureNames) {
      return false;
    } else {
      registeredServices[config.publicProcedureName] = {
        "publicProcedureName" : config.publicProcedureName,
        "procedure" : config.procedure,
        "context" : config.procedure.context,
        "isAsync" : typeof config.isAsynchronous !== "undefined" ?
                      config.isAsynchronous : false,
        "acl" : typeof config.acl !== "undefined" ?
                  config.acl : {whitelist: ["(.*)"], blacklist: []}};
      return true;
    }
  }

  // unregister a previously registered procedure
  function unregister(publicProcedureName) {
    if (publicProcedureName in reservedProcedureNames) {
      return false;
    } else {
      delete registeredServices[publicProcedureName];
      return true;
    }
  }

  // retreive service for a specific procedure name
  function fetchRegisteredService(publicProcedureName){
    return registeredServices[publicProcedureName];
  }

  // receive and execute a pmrpc call which may be a request or a response
  function processPmrpcMessage(eventParams) {
    var serviceCallEvent = eventParams.event;
    var eventSource = eventParams.source;
    var isWorkerComm = typeof eventSource !== "undefined" && eventSource !== null;

    // if the message is not for pmrpc, ignore it.
    if (serviceCallEvent.data.indexOf("pmrpc.") !== 0) {
      return;
    } else {
      var message = decode(serviceCallEvent.data);

      if (typeof message.method !== "undefined") {
        // this is a request

        var newServiceCallEvent = {
          data : serviceCallEvent.data,
          source : isWorkerComm ? eventSource : serviceCallEvent.source,
          origin : isWorkerComm ? "*" : serviceCallEvent.origin,
          shouldCheckACL : !isWorkerComm
        };

        var response = processJSONRpcRequest(message, newServiceCallEvent);

        // return the response
        if (response !== null) {
          sendPmrpcMessage(
            newServiceCallEvent.source, response, newServiceCallEvent.origin);
        }
      } else {
        // this is a response
        processJSONRpcResponse(message);
      }
    }
  }

  // Process a single JSON-RPC Request
  function processJSONRpcRequest(request, serviceCallEvent, shouldCheckACL) {
    if (request.jsonrpc !== "2.0") {
      // Invalid JSON-RPC request
      return createJSONRpcResponseObject(
        createJSONRpcErrorObject(-32600, "Invalid request.",
          "The recived JSON is not a valid JSON-RPC 2.0 request."),
        null,
        null);
    }

    var id = request.id;
    var service = fetchRegisteredService(request.method);

    if (typeof service !== "undefined") {
      // check the acl rights
      if (!serviceCallEvent.shouldCheckACL ||
            checkACL(service.acl, serviceCallEvent.origin)) {
        try {
          if (service.isAsync) {
            // if the service is async, create a callback which the service
            // must call in order to send a response back
            var cb = function (returnValue) {
                       sendPmrpcMessage(
                         serviceCallEvent.source,
                         createJSONRpcResponseObject(null, returnValue, id),
                         serviceCallEvent.origin);
                     };
             // create a errorback which the service
             // must call in order to send an error back
             var eb = function (errorValue) {
                 sendPmrpcMessage(
                   serviceCallEvent.source,
                   createJSONRpcResponseObject(
                		   createJSONRpcErrorObject(
                		     -1, "Application error.",errorValue.message),
                		   null, id),
                   serviceCallEvent.origin);
               };
            invokeProcedure(
              service.procedure, service.context, request.params, [cb, eb, serviceCallEvent]);
            return null;
          } else {
            // if the service is not async, just call it and return the value
            var returnValue = invokeProcedure(
                                service.procedure,
                                service.context,
                                request.params, [serviceCallEvent]);
            return (typeof id === "undefined") ? null :
              createJSONRpcResponseObject(null, returnValue, id);
          }
        } catch (error) {
          if (typeof id === "undefined") {
            // it was a notification nobody cares if it fails
            return null;
          }

          if (error.match("^(No such param)")) {
            return createJSONRpcResponseObject(
              createJSONRpcErrorObject(
                -32602, "Invalid params.", error.message),
              null,
              id);
          }

          // the -1 value is "application defined"
          return createJSONRpcResponseObject(
            createJSONRpcErrorObject(
              -1, "Application error.", error.message),
            null,
            id);
        }
      } else {
        // access denied
        return (typeof id === "undefined") ? null : createJSONRpcResponseObject(
          createJSONRpcErrorObject(
            -2, "Application error.", "Access denied on server."),
          null,
          id);
      }
    } else {
      // No such method
      return (typeof id === "undefined") ? null : createJSONRpcResponseObject(
        createJSONRpcErrorObject(
          -32601,
          "Method not found.",
          "The requestd remote procedure does not exist or is not available."),
        null,
        id);
    }
  }

  // internal rpc service that receives responses for rpc calls
  function processJSONRpcResponse(response) {
    var id = response.id;
    var callObj = callQueue[id];
    if (typeof callObj === "undefined" || callObj === null) {
      return;
    } else {
      delete callQueue[id];
    }

    // check if the call was sucessful or not
    if (typeof response.error === "undefined") {
      callObj.onSuccess( {
        "destination" : callObj.destination,
        "publicProcedureName" : callObj.publicProcedureName,
        "params" : callObj.params,
        "status" : "success",
        "returnValue" : response.result} );
    } else {
      callObj.onError( {
        "destination" : callObj.destination,
        "publicProcedureName" : callObj.publicProcedureName,
        "params" : callObj.params,
        "status" : "error",
        "message" : response.error.message + " " + response.error.data} );
    }
  }

  // call remote procedure
  function call(config) {
    // check that number of retries is not -1, that is a special internal value
    if (config.retries && config.retries < 0) {
      throw new Exception("number of retries must be 0 or higher");
    }

    var destContexts = [];

    if (typeof config.destination === "undefined" || config.destination === null || config.destination === "workerParent") {
      destContexts = [{context : null, type : "workerParent"}];
    } else if (config.destination === "publish") {
      destContexts = findAllReachableContexts();
    } else if (config.destination instanceof Array) {
      for (var i=0; i<config.destination.length; i++) {
        if (config.destination[i] === "workerParent") {
          destContexts.push({context : null, type : "workerParent"});
        } else if (typeof config.destination[i].frames !== "undefined") {
          destContexts.push({context : config.destination[i], type : "window"});
        } else {
          destContexts.push({context : config.destination[i], type : "worker"});
        }
      }
    } else {
      if (typeof config.destination.frames !== "undefined") {
        destContexts.push({context : config.destination, type : "window"});
      } else {
        destContexts.push({context : config.destination, type : "worker"});
      }
    }

    for (var i=0; i<destContexts.length; i++) {
      var callObj = {
        destination : destContexts[i].context,
        destinationDomain : typeof config.destinationDomain === "undefined" ? ["*"] : (typeof config.destinationDomain === "string" ? [config.destinationDomain] : config.destinationDomain),
        publicProcedureName : config.publicProcedureName,
        onSuccess : typeof config.onSuccess !== "undefined" ?
                      config.onSuccess : function (){},
        onError : typeof config.onError !== "undefined" ?
                      config.onError : function (){},
        retries : typeof config.retries !== "undefined" ? config.retries : 5,
        timeout : typeof config.timeout !== "undefined" ? config.timeout : 500,
        status : "requestNotSent"
      };

      isNotification = typeof config.onError === "undefined" && typeof config.onSuccess === "undefined";
      params = (typeof config.params !== "undefined") ? config.params : [];
      callId = generateUUID();
      callQueue[callId] = callObj;

      if (isNotification) {
        callObj.message = createJSONRpcRequestObject(
                    config.publicProcedureName, params);
      } else {
        callObj.message = createJSONRpcRequestObject(
                            config.publicProcedureName, params, callId);
      }

      waitAndSendRequest(callId);
    }
  }

  // Use the postMessage API to send a pmrpc message to a destination
  function sendPmrpcMessage(destination, message, acl) {
    if (typeof destination === "undefined" || destination === null) {
      self.postMessage(encode(message));
    } else if (typeof destination.frames !== "undefined") {
      return destination.postMessage(encode(message), acl);
    } else {
      destination.postMessage(encode(message));
    }
  }

  // Execute a remote call by first pinging the destination and afterwards
  // sending the request
  function waitAndSendRequest(callId) {
    var callObj = callQueue[callId];
    if (typeof callObj === "undefined") {
      return;
    } else if (callObj.retries <= -1) {
      processJSONRpcResponse(
        createJSONRpcResponseObject(
          createJSONRpcErrorObject(
          -4, "Application error.", "Destination unavailable."),
          null,
          callId));
    } else if (callObj.status === "requestSent") {
      return;
    } else if (callObj.retries === 0 || callObj.status === "available") {
      callObj.status = "requestSent";
      callObj.retries = -1;
      callQueue[callId] = callObj;
      for (var i=0; i<callObj.destinationDomain.length; i++) {
        sendPmrpcMessage(
          callObj.destination, callObj.message, callObj.destinationDomain[i], callObj);
        self.setTimeout(function() { waitAndSendRequest(callId); }, callObj.timeout);
      }
    } else {
      // if we can ping some more - send a new ping request
      callObj.status = "pinging";
      var retries = callObj.retries;
      callObj.retries = retries - 1;

      call({
        "destination" : callObj.destination,
        "publicProcedureName" : "receivePingRequest",
        "onSuccess" : function (callResult) {
                        if (callResult.returnValue === true &&
                            typeof callQueue[callId] !== 'undefined') {
                          callQueue[callId].status = "available";
                          waitAndSendRequest(callId);
                        }
                      },
        "params" : [callObj.publicProcedureName],
        "retries" : 0,
        "destinationDomain" : callObj.destinationDomain});
      callQueue[callId] = callObj;
      self.setTimeout(function() {
        if (callQueue[callId] && callQueue[callId].status === "pinging") {
          waitAndSendRequest(callId);
        }
      }, callObj.timeout / retries);
    }
  }

  // attach the pmrpc event listener
  function addCrossBrowserEventListerner(obj, eventName, handler, bubble) {
    if ("addEventListener" in obj) {
      // FF
      obj.addEventListener(eventName, handler, bubble);
    } else {
      // IE
      obj.attachEvent("on" + eventName, handler);
    }
  }

  function createHandler(method, source, destinationType) {
    return function(event) {
      var params = {event : event, source : source, destinationType : destinationType};
      method(params);
    };
  }

  if ('window' in this) {
    // window object - window-to-window comm
    var handler = createHandler(processPmrpcMessage, null, "window");
    addCrossBrowserEventListerner(this, "message", handler, false);
  } else if ('onmessage' in this) {
    // dedicated worker - parent X to worker comm
    var handler = createHandler(processPmrpcMessage, this, "worker");
    addCrossBrowserEventListerner(this, "message", handler, false);
  } else if ('onconnect' in this) {
    // shared worker - parent X to shared-worker comm
    var connectHandler = function(e) {
      //this.sendPort = e.ports[0];
      var handler = createHandler(processPmrpcMessage, e.ports[0], "sharedWorker");
      addCrossBrowserEventListerner(e.ports[0], "message", handler, false);
      e.ports[0].start();
    };
    addCrossBrowserEventListerner(this, "connect", connectHandler, false);
  } else {
    throw "Pmrpc must be loaded within a browser window or web worker.";
  }

  // Override Worker and SharedWorker constructors so that pmrpc may relay
  // messages. For each message received from the worker, call pmrpc processing
  // method. This is child worker to parent communication.

  var createDedicatedWorker = this.Worker;
  this.nonPmrpcWorker = createDedicatedWorker;
  var createSharedWorker = this.SharedWorker;
  this.nonPmrpcSharedWorker = createSharedWorker;

  var allWorkers = [];

  this.Worker = function(scriptUri) {
    var newWorker = new createDedicatedWorker(scriptUri);
    allWorkers.push({context : newWorker, type : 'worker'});
    var handler = createHandler(processPmrpcMessage, newWorker, "worker");
    addCrossBrowserEventListerner(newWorker, "message", handler, false);
    return newWorker;
  };

  this.SharedWorker = function(scriptUri, workerName) {
    var newWorker = new createSharedWorker(scriptUri, workerName);
    allWorkers.push({context : newWorker, type : 'sharedWorker'});
    var handler = createHandler(processPmrpcMessage, newWorker.port, "sharedWorker");
    addCrossBrowserEventListerner(newWorker.port, "message", handler, false);
    newWorker.postMessage = function (msg, portArray) {
      return newWorker.port.postMessage(msg, portArray);
    };
    newWorker.port.start();
    return newWorker;
  };

  // function that receives pings for methods and returns responses
  function receivePingRequest(publicProcedureName) {
    return typeof fetchRegisteredService(publicProcedureName) !== "undefined";
  }

  function subscribe(params) {
    return register(params);
  }

  function unsubscribe(params) {
    return unregister(params);
  }

  function findAllWindows() {
    var allWindowContexts = [];

    if (typeof window !== 'undefined') {
      allWindowContexts.push( { context : window.top, type : 'window' } );

      // walk through all iframes, starting with window.top
      for (var i=0; typeof allWindowContexts[i] !== 'undefined'; i++) {
        var currentWindow = allWindowContexts[i];
        for (var j=0; j<currentWindow.context.frames.length; j++) {
          allWindowContexts.push({
            context : currentWindow.context.frames[j],
            type : 'window'
          });
        }
      }
    } else {
      allWindowContexts.push( {context : this, type : 'workerParent'} );
    }

    return allWindowContexts;
  }

  function findAllWorkers() {
    return allWorkers;
  }

  function findAllReachableContexts() {
    var allWindows = findAllWindows();
    var allWorkers = findAllWorkers();
    var allContexts = allWindows.concat(allWorkers);

    return allContexts;
  }

  // register method for receiving and returning pings
  register({
    "publicProcedureName" : "receivePingRequest",
    "procedure" : receivePingRequest});

  function getRegisteredProcedures() {
    var regSvcs = [];
    var origin = typeof this.frames !== "undefined" ? (window.location.protocol + "//" + window.location.host + (window.location.port !== "" ? ":" + window.location.port : "")) : "";
    for (var publicProcedureName in registeredServices) {
      if (publicProcedureName in reservedProcedureNames) {
        continue;
      } else {
        regSvcs.push( {
          "publicProcedureName" : registeredServices[publicProcedureName].publicProcedureName,
          "acl" : registeredServices[publicProcedureName].acl,
          "origin" : origin
        } );
      }
    }
    return regSvcs;
  }

  // register method for returning registered procedures
  register({
    "publicProcedureName" : "getRegisteredProcedures",
    "procedure" : getRegisteredProcedures});

  function discover(params) {
    var windowsForDiscovery = null;

    if (typeof params.destination === "undefined") {
      windowsForDiscovery = findAllReachableContexts();
      for (var i=0; i<windowsForDiscovery.length; i++) {
        windowsForDiscovery[i] = windowsForDiscovery[i].context;
      }
    } else {
      windowsForDiscovery = params.destination;
    }
    var originRegex = typeof params.originRegex === "undefined" ?
      "(.*)" : params.originRegex;
    var nameRegex = typeof params.nameRegex === "undefined" ?
      "(.*)" : params.nameRegex;

    var counter = windowsForDiscovery.length;

    var discoveredMethods = [];
    function addToDiscoveredMethods(methods, destination) {
      for (var i=0; i<methods.length; i++) {
        if (methods[i].origin.match(new RegExp(originRegex)) &&
            methods[i].publicProcedureName.match(new RegExp(nameRegex))) {
          discoveredMethods.push({
            publicProcedureName : methods[i].publicProcedureName,
            destination : destination,
            procedureACL : methods[i].acl,
            destinationOrigin : methods[i].origin
          });
        }
      }
    }

    pmrpc.call({
      destination : windowsForDiscovery,
      destinationDomain : "*",
      publicProcedureName : "getRegisteredProcedures",
      onSuccess : function (callResult) {
                    counter--;
                    addToDiscoveredMethods(callResult.returnValue, callResult.destination);
                    if (counter === 0) {
                      params.callback(discoveredMethods);
                    }
                  },
      onError : function (callResult) {
                  counter--;
                  if (counter === 0) {
                    params.callback(discoveredMethods);
                  }
                }
    });
  }

  reservedProcedureNames = {"getRegisteredProcedures" : null, "receivePingRequest" : null};

  // return public methods
  return {
    register : register,
    unregister : unregister,
    call : call,
    discover : discover
  };
}();

//AMD suppport
if (typeof define == 'function' && define.amd) {
	define(pmrpc);
}