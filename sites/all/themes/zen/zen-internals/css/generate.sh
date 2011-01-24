#!/bin/sh

# This script is used by the MAINTAINER to generate composite stylesheets for
# the base Zen theme from the stylesheets in the STARTERKIT.

FIXED=( \
  html-reset.css \
  wireframes.css \
  layout-fixed.css \
  page-backgrounds.css \
  tabs.css \
  messages.css \
  pages.css \
  blocks.css \
  navigation.css \
  views-styles.css \
  nodes.css \
  comments.css \
  forms.css \
  fields.css \
  );

STYLESHEET='zen-fixed.css';
echo > $STYLESHEET;
for FILENAME in ${FIXED[*]}; do
  echo >> $STYLESHEET;
  echo "/* $FILENAME */" >> $STYLESHEET;
  echo >> $STYLESHEET;
  cat ../../STARTERKIT/css/$FILENAME >> $STYLESHEET;
done

LIQUID=( \
  html-reset.css \
  wireframes.css \
  layout-liquid.css \
  page-backgrounds.css \
  tabs.css \
  messages.css \
  pages.css \
  blocks.css \
  navigation.css \
  views-styles.css \
  nodes.css \
  comments.css \
  forms.css \
  fields.css \
  );

STYLESHEET='zen-liquid.css';
echo > $STYLESHEET;
for FILENAME in ${FIXED[*]}; do
  echo >> $STYLESHEET;
  echo "/* $FILENAME */" >> $STYLESHEET;
  echo >> $STYLESHEET;
  cat ../../STARTERKIT/css/$FILENAME >> $STYLESHEET;
done

cp ../../STARTERKIT/css/print.css .;

cp ../../STARTERKIT/css/ie.css .;
cp ../../STARTERKIT/css/ie6.css .;
