
var path = Drupal.settings.linkit.url.wysiwyg_fckeditor;
var basePath =  Drupal.settings.basePath;
var modulePath = Drupal.settings.linkit.modulepath;

FCKCommands.RegisterCommand( 'linkit', new FCKDialogCommand( 'linkit', '&nbsp;', path, 580, 320 ) ) ;

var oLinkitItem = new FCKToolbarButton( 'linkit', 'Linkit');
oLinkitItem.IconPath = basePath + modulePath + '/editors/fckeditor/linkit/linkit.png';
FCKToolbarItems.RegisterItem( 'linkit', oLinkitItem );