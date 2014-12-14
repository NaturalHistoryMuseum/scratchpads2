jQuery(document).ready(function () {
	window.surfers = new Array();
	for (var index in Drupal.settings.wavesurfer.surfers){
		var surfer_data = Drupal.settings.wavesurfer.surfers[index];
		window.surfers[index] = Object.create(WaveSurfer);
        window.surfers[index].init({
                container: document.querySelector('#'+surfer_data.container_id),
                waveColor: surfer_data.wavecolor,
                progressColor: surfer_data.progresscolor,
                scrollParent: true,
                minPxPerSec: surfer_data.pixelrate
        });
        window.surfers[index].on('ready', function () {
        	for (var index in Drupal.settings.wavesurfer.onready) {
        		var onarray_f = Drupal.settings.wavesurfer.onready[index];
                eval(onarray_f+'(window.$wavesurfer_id)');
            }
        	var progressDiv = document.querySelector('#progress-bar-'+surfer_data.fileid);
        	progressDiv.style.display = 'none';
        });
        
        window.surfers[index].on('loading', function (percent, xhr) {
        	var progressDiv = document.querySelector('#progress-bar-'+surfer_data.fileid);
        	var progressBar = progressDiv.querySelector('.progress-bar-'+surfer_data.fileid);
        	progressDiv.style.display = 'block';
    	    progressBar.style.width = percent + '%';
        });
        
        window.surfers[index].on('destroy', function () {
        	var progressDiv = document.querySelector('#progress-bar-'+surfer_data.fileid);
        	progressDiv.style.display = 'block';
        });
        window.surfers[index].on('error', function () {
        	var progressDiv = document.querySelector('#progress-bar-'+surfer_data.fileid);
        	progressDiv.style.display = 'block';
        });

        window.surfers[index].loadBuffer(surfer_data.audiofile);       
	}
});

function wavesurfer_playsurfer(index) {
       window.surfers[index].playPause();
       window.surfers[index].playPause();
       window.surfers[index].playPause();
}