QUICK START GUIDE
-----------------
1. Create a View.
2. Add a "Bulk operations" field if available (see technical details below).
3. Configure the field. There's a "Views Bulk Operations" fieldset where the
actions visible to the user are selected.
4. Go to the View page. VBO functionality should be present.

TECHNICAL DETAILS
-----------------
The module relies on the Views 3 "form" feature available in all Views releases after May 28th 2011.
The selection field (checkbox / radio) is provided as a views field called "Bulk operations".
The field can use a relationship, allowing you to have a node view with
operations on node authors, for example.
Note that currently a View can only have one VBO field.
Even if the view has several VBO fields, only the first one found will be used.

The "Bulk operations" views field is available in Views for all entity base tables,
since VBO relies on entities directly and no longer has its own "object" abstraction.

There is Drush integration available, allowing you to run an action against the
resultset of any View that has the VBO field in its master display.

Operations are gathered from two sources:
1) Drupal Core Action API
2) Rules 2 (preferred).
The module also allows to use Batch API or the Drupal Queue to process the selected entities, in order to avoid timeouts.

RULES 2 INTEGRATION
-------------------
The module can execute any created Rules component (rule, ruleset, action set)
that accepts an entity (example types: "node", "entity") as the first parameter.
As a reminder, Rules components can be created at admin/config/workflow/rules/components.

EXAMPLE VIEWS
-------------
VBO comes with two default views, reimplementing the Content and User listings.
They are disabled by default. After enabling them at admin/structure/views
they can be accessed at admin/content2 and admin/people2.
