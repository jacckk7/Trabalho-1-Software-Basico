SECTION TEXT
    INPUT OLD_DATA
    LOAD      OLD_DATA
L1: DIV dois
    STORE  NEW_DATA
;sdsdxfedfsfedf
    MUL dois
    STORE TMP_DATA                                         ;     ds dowsjhdis
    LOAD OLD_DATA
    SUB TMP_DATA




        STORE TMP_DATA
    OUTPUT TMP_DATA
    COPY NEW_DATA   , OLD_DATA
    LOAD OLD_DATA
    JMPP L1
    STOP

SECTION DATA
dois: 
CONST 2
OLD_DATA:    SPACE
NEW_DATA: SPACE
TMP_DATA: SPACE