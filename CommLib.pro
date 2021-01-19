TEMPLATE = subdirs
 
SUBDIRS = \
          lib \
          example  

# where to find the sub projects - give the folders
lib.subdir = MetaCommLib
example.subdir  = Example

# what subproject depends on others
example.depends = lib
