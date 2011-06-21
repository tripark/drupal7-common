(function ($) {
  Drupal.behaviors.vbo = {
    attach: function(context) {
      // Add the table behavior for each VBO form found on this page.
      $('.vbo-views-form', context).each(function() {
        // Table-based styles are special and have their own behaviors.
        if ($('.views-table', this).length) {
          Drupal.vbo.initTableBehaviors(this);
        }
        else {
          Drupal.vbo.initGenericBehaviors(this);
        }
      });
    }
  }

  Drupal.vbo = Drupal.vbo || {};
  Drupal.vbo.initTableBehaviors = function(form) {
    // Move the select all markup from the hidden div to a table row.
    if ($('.vbo-select-all-markup', form).length) {
      $('tbody', form).prepend('<tr class="views-row-select-all even">></tr>');
      var colspan = $('table th', form).length;
      $('.views-row-select-all', form).html('<td colspan="' + colspan + '">' + $('.vbo-select-all-markup', form).html() + '</td>');
    }

    $('.vbo-select-all', form).show();
    // Set up the "select all" functionality.
    $('.vbo-select-all', form).click(function() {
      $('input[id^="edit-views-bulk-operations"]', form).attr('checked', this.checked);
      $('.views-row-select-all', form).toggle();
    });
    $('.vbo-select-all-pages', form).click(function() {
      $('.vbo-this-page', form).hide();
      $('.vbo-all-pages', form).show();
      $('.select-all-rows', form).val('1');
      return false;
    });
    $('.vbo-select-this-page', form).click(function() {
      $('.vbo-all-pages', form).hide();
      $('.vbo-this-page', form).show();
      $('.select-all-rows', form).val('0');
      return false;
    });

    // Set up the ability to click anywhere on the row to select it.
    $('tbody tr', form).click(function(event) {
      if (event.target.tagName.toLowerCase() != 'input' && event.target.tagName.toLowerCase() != 'a') {
        $('input[id^="edit-views-bulk-operations"]', this).each(function() {
          var checked = this.checked;
          // trigger() toggles the checkmark *after* the event is set,
          // whereas manually clicking the checkbox toggles it *beforehand*.
          // that's why we manually set the checkmark first, then trigger the
          // event (so that listeners get notified), then re-set the checkmark
          // which the trigger will have toggled. yuck!
          this.checked = !checked;
          $(this).trigger('click');
          this.checked = !checked;
        });
      }
    });
  }

  Drupal.vbo.initGenericBehaviors = function(form) {
    // Show the "select all" fieldset.
    $('.vbo-select-all-markup', form).show();

    $('.vbo-select-this-page', form).click(function() {
      $('input[id^="edit-views-bulk-operations"]', form).attr('checked', this.checked);
      $('.vbo-select-all-pages', form).attr('checked', false);
    });
    $('.vbo-select-all-pages', form).click(function() {
      $('input[id^="edit-views-bulk-operations"]', form).attr('checked', this.checked);
      if (this.checked) {
        $('.vbo-select-this-page', form).attr('checked', false);
        $('.select-all-rows', form).val('1');
      }
      else {
        $('.select-all-rows', form).val('0');
      }
    });
  }

})(jQuery);
