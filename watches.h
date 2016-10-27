#ifdef __cplusplus
extern"C"
{
#endif

double wtc_ctime();
double wtc_wtime();
void wtc_init();
void wtc_set_name(const int idx, const char *name);
void wtc_change_watch(const int idx);
void wtc_print_watches();


#ifdef __cplusplus
}
#endif

