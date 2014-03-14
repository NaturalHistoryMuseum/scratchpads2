(function($){
  $(document).ready(function(){
    pmrpc.register({publicProcedureName: "reply", procedure: function(status, results){
      interaction_reply(status, results);
      return "OK";
    }});
    pmrpc.register({publicProcedureName: "getParameterValue", procedure: function(parameterName){
      return getParameterValue(parameterName);
    }});
    pmrpc.register({publicProcedureName: "getInputData", procedure: function(){
      return inputData;
    }});
    pmrpc.register({publicProcedureName: "getWorkflowRunId", procedure: function(){
      return "";
    }});
    pmrpc.register({publicProcedureName: "setTitle", procedure: function(title){
      return "OK";
    }});
    $('.taverna-dialogue').dialog({title: Drupal.settings.taverna.interaction.title, modal: true, width: 2 * (window.innerWidth / 3)});
  });
})(jQuery);

function interaction_reply(status, results){
  var outputData = JSON.stringify(results);
  jQuery.ajax({url: Drupal.settings.taverna.interaction.callback_url, type: "POST", async: false, data: outputData, headers: {"X-Taverna-Interaction-Reply": escape(status)}});
  jQuery('.taverna-dialogue').dialog('close');
  return false;
}