
int& bad() { int local = 7; return local; }
int main() { return bad(); }
