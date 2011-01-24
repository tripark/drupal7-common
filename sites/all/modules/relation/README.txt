/* $Id: README.txt,v 1.2 2010/11/24 20:57:49 sun Exp $ */

-- SUMMARY --

Describes relationships between entities.

The intent of this module is to provide an API that describes the relationships
between Drupal entities. For example:
  comments <--> nodes
  nodes <--> authors
  terms <--> nodes
  files <--> users
  files <--> nodes

Some of these relationships are hard-coded properties of an entity; for example,
nodes have an author, a creation date, and a last updated date. Other
relationships exist because of field values; putting a filefield on a node type
creates a relationship between file entities and node type entities. An
entity_reference module could explicitly define relationships between entities.

This module treats entity relationships as an entity type themselves. Types of
relationships are bundles. Each bundle describes the predicate of an RDF triple;
the two entites are the subject and object.

  Entity relationship type    = SUBJECT   + PREDICATE      + OBJECT
  Node author relationship    = node      + creator        + user
  Taxonomy field relationship = blog post + is tagged with + some term

There is a distinction between relationships that are defined by the properties
or behavior of an entity type and relationships that are defined per bundle. We
think we can relate these ways:

  entity type <--> entity type
  bundle <--> bundle

But not these:
  entity type <--> bundle

...because entity types are not extensible in the way that bundles are
extensible via the core Field API. Entity type relationships will need to be
pre-defined, since they're infrastructure; we can describe them but not extend
them. Bundle relationships should be inferred from whatever field configuration
is present on any particular Drupal installation, and users will be able to
create and edit relationships between bundles.

An entity relationship API will let us visualize the content model of a Drupal
site. With this, we could export an RDF schema of the entire content model of a
site; we could build a "content explorer" that shows the linkings from any one
piece of content (a user -> their nodes -> a particular node -> a term -> nodes
tagged with that term). The first milestone for this module is to provide simple
blocks that display specific corners of this graph, like a user's nodes, and
terms a user has used. In the future, we would like to be able to add filters to
the graph (like "a users nodes" + "only blog posts").

Drupal core defines several entity types:
- user
- node
- comment
- taxonomy
- file (within system.module's hook_entity_info())

Some related discussions/projects:
- http://drupal.org/node/533222 (nodereference/userreference fields in D7)
- http://drupal.org/project/entity (Entity API)
- http://drupal.org/project/commentreference (CCK Comment Reference)
- http://drupal.pastebin.com/avnKvCD0 (notes from chx)

A little more thinking/rephrasing...

Relationship data may not be stored by awesomerelationships itself;
awesomerelationships will provide a way for entity or field providers to
describe any relationships they may create. This way, we can query
awesomerelationships' relationship API about any relationship on the site.

The awesomerelationships module should be able to ask the relationship bundle
provider things like:
- what is your subject type + predicate + object type?
- what relationships of your type exist, given a set of subject or object ids?
  -> what relationships of your type exist for subject id *?
  -> what relationships of your type exist for object id (123, 456, 78, 9)?

We will be able to ask awesomerelationships' relationship API about what
relationships exist, like "given entity type X, find all relationship bundles
where X is the subject (or object, or either)".

The relationship API will find the shortest path between entities:
  get_relationship('subject_type', 'object_type')
  get_relationship(user, node)  ~~~> user --> node
  get_relationship(user, term) ~~~> user --> node --> term
  get_relationship(user, file, term) ~~~> user --> file --> node --> term


For a full description of the module, visit the project page:
  http://drupal.org/project/relation
To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/relation


-- REQUIREMENTS --

* @todo


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* @todo


-- USAGE --

* @todo


-- CONTACT --

Current maintainers:
* Daniel F. Kudwien (sun) - http://drupal.org/user/54136

