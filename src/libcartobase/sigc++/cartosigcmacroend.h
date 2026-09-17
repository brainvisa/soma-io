/* protect emit function of sigc++ against Qt emit macro
*/

#undef emit
#undef slots

#ifdef QOBJECTDEFS_H
#define emit
#define slots
#endif
