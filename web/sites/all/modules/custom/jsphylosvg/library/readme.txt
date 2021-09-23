This software is distributed under the GPL License.

Copyright (c) Samuel Smits, samsmits@gmail.com
All rights reserved.

If you use this software, please cite:
Smits SA, Ouverney CC, 2010. jsPhyloSVG: A Javascript Library for Visualizing Interactive and Vector-Based Phylogenetic Trees on the Web. 
PLoS ONE 5(8): e12267. doi:10.1371/journal.pone.0012267


Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
Neither the name of the Samuel Smits nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.



DOCUMENTATION
-------------
Available on our site, www.jsPhyloSVG.com


INSTALLATION NOTES
------------------
jsPhyloSVG leverages the SVG rendering library Raphael. This need to be included in your HTML.
Here is an example of an HTML skeleton with the includes:

<html>
<head>				
	<script type="text/javascript" src="js/raphael/raphael-min.js"></script> 
	<script type="text/javascript" src="js/jsphylosvg-min.js"></script> 				
</head>
<body>
	<div id="svgCanvas"> </div>
</body>
</html>		


VERSION UPDATES
---------------
1.55 - Event handlers updated for compatibility in Internet Explorer
1.54 - Newick re-instantiation (thanks Zack Whedbee)
1.53 - NeXML handling (including Rutger Vos' contribution)
1.52 - Reconfigure branch position calculations, optimization
1.51 - Internet Explorer optimization
1.5 - Significant update, includes internal ribbons and other styling.
1.29 - Ability to export svg source.
1.28 - Highlight labels with bgStyle.
1.27 - Ability to apply styles on name/label nodes. Extends the charting's styling approach. Akin to CSS.
1.26b - Img tag support in annotations.
1.26 - Revised internal processing for interlibrary operability
1.25d - Added showScaleBar parameter for displaying scale bars
1.25c - Support for <confidence> in phyloXML
1.25b - Added alignPadding parameter to push labels out further.
1.25 - Added NeXML support. Adjustment to inner label locations.
1.24 - Ability to add titles and uri links to bootstrap labels.
1.23 - Allow for simpler render style updates. 
1.22 - Stems are now centered to children in phylograms for visual clarity.
1.21 - Added Dendrogram capabilities (lengthless nodes).