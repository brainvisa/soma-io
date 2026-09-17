/* protect emit function of sigc++ against Qt emit macro
*/

#ifdef emit
#undef emit
#endif
#ifdef slots
#undef slots
#endif
#define emit sigcemit
#define slots sigcslots
