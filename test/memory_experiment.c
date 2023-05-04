int main() {
	*(long *)0xF0000010 = 111;
	return *(long *)0xF0000010;
}