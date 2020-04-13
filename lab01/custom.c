int custom(int user_input)
{
	int result = 1;
	int iterations = 5;

	for (int i = 0; i <= iterations; ++i)
	{
		result = result * user_input;
	}

	return result;
}
