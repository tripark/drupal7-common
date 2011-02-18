Drupal.behaviors.tpcGeneral = {
  attach: function(context) {
  // Alter CKEditor's link dialog to make the protocol setting more obvious.
    if (typeof(CKEDITOR) !== "undefined") {
      CKEDITOR.on( 'dialogDefinition', function( ev ) {
        // Take the dialog name and its definition from the event data.
        var dialogName = ev.data.name;
        var dialogDefinition = ev.data.definition;
      
        // Check if the definition is from the dialog we're
        // interested on (the "Link" dialog).
        if ( dialogName == 'link' ) {
          // Get a reference to the "Link Info" tab.
          var infoTab = dialogDefinition.getContents( 'info' );
      
          // Set the default value for the protocol field.
          var protocolField = infoTab.get( 'protocol' );
          protocolField['label'] = "Path type";
          protocolField['items'][4][0] = "(relative)";
          protocolField['default'] = '';
        }
      });
    }
  }
}
