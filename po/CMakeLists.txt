if(NOT DEFINED PO_DIRECTORY)
    set(PO_DIRECTORY ".")
endif()

if(NOT DEFINED PROJECT_ID)
    set(PROJECT_ID "mrim-prpl-underbush")
endif()

if(NOT DEFINED PO_PROJECT)
    set(PO_PROJECT "${PO_DIRECTORY}/${PRJID}")
endif()

file(STRINGS "po.list" PO_LIST)
gettext_create_translations("${PROJECT_ID}.pot" ALL ${PO_LIST})
