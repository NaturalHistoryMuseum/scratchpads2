/**
 * WebSocket with graceful degradation - jQuery plugin
 * @author David Lindkvist
 * @version 0.1
 * 
 * Returns an object implementing the WebSocket API. 
 * 
 * If browser supports WebSockets a native WebSocket instance is returned. 
 * If not, a simulated half-duplex implementation is returned which uses polling 
 * over HTTP to retrieve new messages
 * 
 * OPTIONS
 * -----------------------------------------------------------------------------
 * 
 * {Number}		fallbackOpenDelay		number of ms to delay simulated open 
 * 										event for fallback
 * {Number}		fallbackPollInterval	number of ms between requests for 
 * 										fallback polling
 * {Object}		fallbackPollParams		optional params to pass with each poll 
 * 										requests
 * 
 * EXAMPLES
 * -----------------------------------------------------------------------------
 * 
 * 	var websocket = $.gracefulWebSocket("ws://127.0.0.1:8080/");
 * 
 * 	var websocket = $.gracefulWebSocket({
 * 		fallbackPollParams:  {
 * 			"latestMessageID": function () {
 * 				return latestMessageID;
 * 			}
 *  	} 
 * 	});
 * 
 */

(function ($) {

	$.extend({
	
		gracefulWebSocket: function (url, options) {
		
			// Default properties 
			this.defaults = {
				keepAlive: false,		// not implemented - should ping server to keep socket open
				autoReconnect: false,	// not implemented - should try to reconnect silently if socket is closed
				fallback: true,			// not implemented - always use HTTP fallback if native browser support is missing
				fallbackSendURL: url.replace('ws:', 'http:').replace('wss:', 'https:'),
				fallbackSendMethod: 'POST',
				fallbackPollURL: url.replace('ws:', 'http:').replace('wss:', 'https:'),
				fallbackPollMethod: 'GET',
				fallbackOpenDelay: 100,	// number of ms to delay simulated open event
				fallbackPollInterval: 3000,	// number of ms between poll requests
				fallbackPollParams: {}		// optional params to pass with poll requests
			};
			
			// Override defaults with user properties
			var opts = $.extend({}, this.defaults, options);
			
			/**
			 * Creates a fallback object implementing the WebSocket interface
			 */
			function FallbackSocket() {
				
				// WebSocket interface constants
				const CONNECTING = 0;
				const OPEN = 1;
				const CLOSING = 2;
				const CLOSED = 3;
				
				var pollInterval;
				var openTimout;
				
				// create WebSocket object
				var fws = {
					// ready state
					readyState: CONNECTING,
					bufferedAmount: 0,
					send: function (data) {
						var success = true;
						$.ajax({
							async: false, // send synchronously
							type: opts.fallbackSendMethod,
							url: opts.fallbackSendURL + '?' + $.param( getFallbackParams() ),
							data: data,
							dataType: 'text',
							contentType : "application/x-www-form-urlencoded; charset=utf-8",
							success: pollSuccess,
							error: function (xhr) {
								success = false;
								$(fws).triggerHandler('error');
							}
						});
						return success;
					},
					close: function () {
						clearTimeout(openTimout);
						clearInterval(pollInterval);
						this.readyState = CLOSED;
						$(fws).triggerHandler('close');
					},
					onopen: function () {},
					onmessage: function () {},
					onerror: function () {},
					onclose: function () {},
					previousRequest: null,
					currentRequest: null
				};
				
				function getFallbackParams() {
					
					// update timestamp of previous and current poll request
					fws.previousRequest = fws.currentRequest;					
					fws.currentRequest = new Date().getTime();
					
					// extend default params with plugin options
					return $.extend({"previousRequest": fws.previousRequest, "currentRequest": fws.currentRequest}, opts.fallbackPollParams);
				}
				
				/**
				 * @param {Object} data
				 */
				function pollSuccess(data) {
					
					// trigger onmessage
					var messageEvent = {"data" : data};
					fws.onmessage(messageEvent);	
				}
				
				function poll() {
					
					$.ajax({
						type: opts.fallbackPollMethod,
						url: opts.fallbackPollURL,
						dataType: 'text',
						data: getFallbackParams(),
						success: pollSuccess,
						error: function (xhr) {			
							$(fws).triggerHandler('error');
						}
					});		
				}
				
				// simulate open event and start polling
				openTimout = setTimeout(function () { 
					fws.readyState = OPEN;
					//fws.currentRequest = new Date().getTime();
					$(fws).triggerHandler('open');
					poll();
					pollInterval = setInterval(poll, opts.fallbackPollInterval);
					
				}, opts.fallbackOpenDelay);
				
				// return socket impl
				return fws;
			}
			
			// create a new websocket or fallback
			var ws = window.WebSocket ? new WebSocket(url) : new FallbackSocket();
	 		$(window).unload(function () { ws.close(); ws = null });
			return ws;
		}
	});
	
})(jQuery);