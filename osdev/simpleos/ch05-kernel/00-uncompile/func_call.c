int callee(int arg)
{
	return arg;
}

void caller()
{
	callee(0xdede);
}
