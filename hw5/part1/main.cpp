#include <iostream>
#include <set>
#include <utility>

using namespace std;

typedef pair<int, int> pii;

int require[] = {1,2,3,4, 2,1,5,6, 2,1,2,3, 7,6,3,2, 1,2,3,6};

int main()
{
	for (int frameN = 1; frameN <= 7; ++frameN)
	{
		set<pii> S;
		int t = 0;
		int pgfault = 0;

		for (auto p : require)
		{
			//printf("page in %d\n", p);
			bool isfind = false;
			auto ptr = S.begin();
			for (auto it = S.begin(); it != S.end(); ++it)
			{
				if ((*it).second == p)
				{
					isfind = true;
					ptr = it;
				}
			}

			if (!isfind)
			{
				++pgfault;
				S.emplace(t++, p);
			}
			else
			{
				S.erase(ptr);
				S.emplace(t++, p);
			}

			if (S.size() == frameN + 1)
				S.erase(S.begin());
			
			/*
			for (auto x : S)
				printf("t = %d, p = %d\n", x.first, x.second);
			puts("");
			*/			
		}

		printf("%d\n", pgfault);
	}
	return 0;
}
