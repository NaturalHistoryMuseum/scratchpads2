(function($){
  /**
   * commandHandler
   *
   * This class is used to handle and stack multiple commands
   * comming from the editor.
   *
   * There are three types of command:
   * - 'reload': This command is used to reload the data grid.
   *             Only one such command can be in the queue at any
   *             one time, such that if a new one is added it will
   *             overwrite any existing one.
   *
   *             When a 'reload' is executing, all other commands
   *             in the queue have to wait for it's completion.
   *
   * - 'update-cell': This command is used to update the data in a
   *             slickgrid cell. When added, such commands will
   *             float up ahead of any 'reload' commands.
   *
   *             When an 'update-cell' command is executing, other
   *             'update-cell' and 'reload' commands in the queue
   *             have to wait for it's completion.
   *
   * - 'update-setting': This commands is used to update a Slickgrid
   *             setting. When added such commands will float up
   *             ahead of any 'reload' commands.
   *
   *             When an 'update-setting' command is execute, other
   *             'update-setting' and 'reload' commands in the queue
   *             have to wait for it's completion.
   *
   */
  Slick.Data.CommandHandler = function commandHandler(){
    /**
     * init
     */
    this.init = function(){
      this.queues = {
        reload: [],
        updateCell: [],
        updateSetting: [],
        pausedReload: []
      };
      this.runningCommands = {
        reload: {},
        updateCell: {},
        updateSetting: {}
      };
      this.reloadPaused = false;
      this.dispatchTriggered = false;
      this.nextCommandID = 0;
    }

    /**
     * addCommand
     *
     * Adds a new command to be executed. 
     *
     * 'data' is the object that will be passed to jQuery.ajax. In addition
     * this object can defined a 'slickCommandStart' function that will
     * be invoked when the command is being run.
     *
     */
    this.addCommand = function(type, data){
      if (type == 'reload'){
        if (this.reloadPaused){
          this.queues.pausedReload = [data];
        } else {
          this.queues.reload = [data];
        }
      } else if (type == 'update-cell'){
        this.queues.updateCell.push(data);
      } else if (type == 'update-setting'){
        this.queues.updateSetting.push(data);
      }
      this.dispatch();
    }

    /**
     * pauseReload
     *
     * Pause any reload operation.
     */
    this.pauseReload = function(){
      if (!$.isEmptyObject(this.runningCommands.reload)){
        for (var i in this.runningCommands.reload){
          if (this.runningCommands.reload.hasOwnProperty(i)){
            this.queues.pausedReload.push(this.runningCommands.reload[i]);
            delete this.runningCommands.reload[i];
          }
        }
      } else if (this.queues.reload.length > 0){
        this.queues.pausedReload = this.queues.reload;
        this.queues.reload = [];
      }
      this.reloadPaused = true;
    }

    /**
     * resumeReload
     *
     * Resume any reload operation
     */
    this.resumeReload = function(){
      this.queues.reload = this.queues.pausedReload;
      this.queues.pausedReload = [];
      this.reloadPaused = false;
      this.dispatch();
    }

    /**
     * cancelReload
     *
     * Cancel any running, queued or paused reload operation
     */
    this.cancelReload = function(){
      this.queues.pausedReload = [];
      this.queues.reload = [];
      this.runningCommands.reload = {};
    }

    /**
     * dispatch
     *
     * Run any command in the queue that may be run. 
     *
     * If now is defined and true, then the dispatch is run instantly,
     * otherwise it is defered to the end of the execution frame.
     *
     * This allows the proper bubbling of events.
     */
    this.dispatch = function(now){
      if (typeof now == 'undefined' || !now){
        if (!this.dispatchTriggered){
          this.dispatchTriggered = true;
          window.setTimeout(jQuery.proxy(this, 'dispatch', true), 0);
        }
      } else {
        this.dispatchTriggered = false;
        if (!$.isEmptyObject(this.runningCommands.reload)){
          return;
        }
        if (this.queues.updateCell.length > 0 && $.isEmptyObject(this.runningCommands.updateCell)){
          this.runCommand('updateCell', this.queues.updateCell.shift());
        }
        if (this.queues.updateSetting.length > 0 && $.isEmptyObject(this.runningCommands.updateSetting)){
          this.runCommand('updateSetting', this.queues.updateSetting.shift());
        }
        if ($.isEmptyObject(this.runningCommands.updateCell) && $.isEmptyObject(this.runningCommands.updateSetting) && this.queues.reload.length > 0){
          this.runCommand('reload', this.queues.reload.shift());
        }
      }
    }

    /**
     * runCommand
     *
     * Run a particular command
     */
    this.runCommand = function(type, args){
      // Generate a unique id for this command and store it
      var count = this.nextCommandID;
      var id = type + count.toString();
      while(typeof this.runningCommands[type][id] !== 'undefined'){
        count = count + 1;
        id = type + count.toString();
      }
      this.nextCommandID = count + 1;
      this.runningCommands[type][id] = args;
      // Clone the command and add ouw own success/error wrappers
      args = jQuery.extend({}, args);
      if (typeof args.slickCommandStart !== 'undefined'){
        args.slickCommandStart();
      }
      args.success = jQuery.proxy(this, 'commandSuccess', type, id, args.success);
      args.error = jQuery.proxy(this, 'commandError', type, id, args.error);
      jQuery.ajax(args);
    }

    /**
     * commandSuccess
     *
     * AJAX callback
     */
    this.commandSuccess = function(type, id, callback, data, status, xhr){
      if (typeof this.runningCommands[type][id] !== 'undefined'){
        delete this.runningCommands[type][id];
        if (typeof callback !== 'undefined'){
          callback(data, status, xhr);
        }
        this.dispatch();
      }
    }

    /**
     * commandError
     *
     * AJAX callback
     */
    this.commandError = function(type, id, callback, xhr, status, error){
      if (typeof this.runningCommands[type][id] !== 'undefined'){
        delete this.runningCommands[type][id];
        if (typeof callback !== 'undefined'){
          callback(xhr, status, error);
        }
        this.dispatch();
      }
    }
    
    this.init();
  }
})(jQuery);
