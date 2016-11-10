#ifdef __cplusplus
extern"C"
{
#endif

int wtc_printf(const char *format, ...);
int wtc_dateprintf(const char *format, ...);
void wtc_open(const int rank, const char* const hname);
void wtc_close();

double wtc_ctime();
double wtc_wtime();
void wtc_init();
void wtc_set_name(const int idx, const char* const name);
void wtc_change_watch(const int idx);
void wtc_print_watches();


#ifdef __cplusplus
}
#endif
