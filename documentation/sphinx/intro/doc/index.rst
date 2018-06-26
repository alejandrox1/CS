.. _rst_tutorial:



############################################
Documenting Software with Sphinx and Autodoc
############################################

.. topic:: Overview

    This page is simple to show how to use rst.
    It is based on `RST syntax guide <https://thomas-cokelaer.info/tutorials/sphinx/rest_syntax.html#code-block-directive>`_.

    .. toctree::
        :maxdepth: 2
        :caption: Contents:

        api

    :Data: |today|
    :Author: alejandrox1

.. contents::
    :depth: 3

.. image:: images/tacc.png                                                  
    :width: 200px                                                           
    :height: 100px                                                          
    :align: center


Welcome to trianglelib's documentation!                                         
#######################################                                         

..
.. toctree::                                                                    
   :maxdepth: 2                                                                 
   :caption: Contents:                                                          
                                                                                
          
Application-developer documentation
-----------------------------------

The user-space API manual gathers together documents describing aspects of
the kernel interface as seen by application developers.

.. toctree::
   :maxdepth: 2

   userspace-api/index
                                                                                
Indices and tables                                                              
##################                                         
                                                                                
* :ref:`genindex`                                                               
* :ref:`modindex`                                                               
* :ref:`search`




Text Formatting
###############

Inline Markup and Special Characters
====================================

======== ================================================================= ==========================================================
usage       syntax HTML                                                       rendering
======== ================================================================= ==========================================================
Italic    `*Italic*` *Italic*
Bold      `**Bold*`` **Bold**
Link      ```chaos <https://en.wikipedia.org/wiki/Chaos_theory>`_``         `chaos <https://en.wikipedia.org/wiki/Chaos_theory>`_
Verbatim  ````type Writer struct {}````                                     `type Writer struct {}`
======== ================================================================= ==========================================================


Headings
========

.. code-block:: rest

    *****
    Title
    *****

    subtitle
    ########

    subsubtitle
    **********************
    and so on

Normally, there are no heading levels assigned to certain characters as the
structure is determined from the succession of headings. However, it is better to stick to the same convention throughout a project. For instance:

* `#` with overline, for parts
* `*` with overline, for chapters
* `=`, for sections
* `-`, for subsections
* `^`, for subsubsections
* `"`, for paragraphs


Code blocks are introduced by a double-colon and are indented::
   
    import docutils
    print help(docutils)

>>> print 'But doctests start with ">>>" and need no indentation.'



.. note::
   Your note should consist of one or more paragraphs, all indented
   so that they clearly belong to the note and not to the text or
   directive that follows.

   Many other directives are also supported, including: warning,
   versionadded, versionchanged, seealso, deprecated, rubric,
   centered, hlist, glossary, productionlist.



.. code-block:: c

   /* Or say "highlight::" once to set the language for all of the
   code blocks that follow it. Options include ":linenos:",
   ":linenothreshold:", and ":emphasize-lines: 1,2,3". */

   char s[] = "You can also say 'python', 'ruby', ..., or 'guess'!";



.. literalinclude:: example.py
   :lines: 5-22
   :emphasize-lines: 4,18



.. module:: httplib

.. class:: Request
   Zero or more paragraphs of introductory material for the class.

   .. method:: send()

   Description of the send() method.

    .. attribute:: url

    Description of the url attribute.
    Many more members are possible than just method and attribute,
    and non-Python languages are supported too; see the Sphinx docs
    for more possibilities!




.. testcode::

   print 'The doctest extension supports code without >>> prompts!'

.. testoutput::
   
   The doctest extension supports code without >>> prompts!
