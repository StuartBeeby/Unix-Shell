void histCountIncrement(int *histcount) 
{
	*histcount = *histcount + 1;
	if(*histcount > 100){
		*histcount = 0;
	}
	return;
}