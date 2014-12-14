jQuery(document).ready(function () {
	window.surfers = new Array();
	jQuery('.wavesurfer-container').each(function () {
		var index = jQuery(this).data("fileid");
		window.surfers[index] = Object.create(WaveSurfer);
        window.surfers[index].init({
                container: this,
                waveColor: jQuery(this).data("wavecolor"),
                progressColor: jQuery(this).data("progresscolor"),
                scrollParent: true,
                minPxPerSec: jQuery(this).data("pixelrate"),
                fileid: index
        });
        window.surfers[index].on('ready', function () {
        	for (var index2 in Drupal.settings.wavesurfer.onready) {
        		var onarray_f = Drupal.settings.wavesurfer.onready[index2];
                eval(onarray_f+'(window.surfers['+index+'])'); 
            }
        	var progressDiv = document.querySelector('#progress-bar-'+index);
        	progressDiv.style.display = 'none';
            window.surfers[index].seekTo(0);
        });
        
        window.surfers[index].on('loading', function (percent, xhr) {
        	var progressDiv = document.querySelector('#progress-bar-'+index);
        	var progressBar = progressDiv.querySelector('.progress-bar-'+index);
        	progressDiv.style.display = 'block';
    	    progressBar.style.width = percent + '%';
        });
        
        window.surfers[index].on('destroy', function () {
        	var progressDiv = document.querySelector('#progress-bar-'+index);
        	progressDiv.style.display = 'block';
        });
        window.surfers[index].on('error', function () {
        	var progressDiv = document.querySelector('#progress-bar-'+index);
        	progressDiv.style.display = 'block';
        });

        window.surfers[index].loadBuffer(jQuery(this).data("audiofile"));       
	});
});

function wavesurfer_playsurfer(index) {
       window.surfers[index].playPause();
}