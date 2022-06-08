#include <iostream>
#include <random>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#define N 5
#define A 50
#define TIME_MIN 2
#define TIME_MAX 10
using namespace std;
float calculate(vector<float> a, vector<string> b, int n);

int main()
{
	random_device rd;
	mt19937 gen(rd());

	int n, k, m;

	cin >> n >> k >> m; // 열 개수, 연산자 개수, 숫자 개수

	vector<char> operat(k);
	vector<string> random_operat(m - 1);
	vector<float> random_number(m);
	vector<float> arr(N);
	for (int i = 0; i < k; i++)
		cin >> operat[i]; // 연산자 입력

	int maximum = pow(10, 2) - 1;
	uniform_int_distribution<int> time_dir(TIME_MIN, TIME_MAX);
	uniform_real_distribution<float> result_dir(0, maximum);
	uniform_int_distribution<int> operat_index(0, k - 1);

	for (int i = 0; i < n; i++) // 출력
	{
		bool inMax = false;
		float result = result_dir(gen);
		cout << time_dir(gen) << ' ' << result << ' ';

		float offset = result;
		if (offset < A)
			offset = A;
		else if (offset > A)
			offset = maximum - A;

		uniform_real_distribution<float> number_dir(offset - A, offset + A);
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < m; j++)
				random_number[j] = number_dir(gen);
			for (int j = 0; j < m - 1; j++)
				random_operat[j] = operat[operat_index(gen)];

			float sum = calculate(random_number, random_operat, m);
			if (sum > result)
				inMax = true;
			while (i == N - 1 && result >= sum && !inMax) // 답보다 큰 값이 하나도 없을 때
			{
				for (int j = 0; j < m; j++)
					random_number[j] = number_dir(gen);
				for (int j = 0; j < m - 1; j++)
					random_operat[j] = operat[operat_index(gen)];
				sum = calculate(random_number, random_operat, m);
			}
			arr[i] = sum;
			cout << fixed;
			cout.precision(0); // 소수점 출력 형식
			for (int j = 0; j < m - 1; j++)
				cout << random_number[j] << random_operat[j];
			cout << random_number[m - 1] << ' ';
		}
		cout << fixed;
		cout.precision(0); // 소수점 출력 형식
		for (int i = 0; i < N; i++)
			cout << arr[i] << ' ';
		cout << endl;
	}

	return 0;
}
float calculate(vector<float> a, vector<string> b, int n)
{
	queue<string> s;
	s.push(to_string(a[0]));
	for (int i = 0; i < n - 1; i++)
	{
		if (b[i] == "+" || b[i] == "-")
		{
			s.push(to_string(a[i + 1]));
			s.push(b[i]);
		}
		else if (b[i] == "*" || b[i] == "/")
		{
			stack<string> t;
			while (s.front() == "+" || s.front() == "-")
			{
				t.push(s.front());
				s.pop();
			}
			s.push(to_string(a[i + 1]));
			s.push(b[i]);
			while (t.size())
			{
				s.push(t.top());
				t.pop();
			}
		}
	}

	stack<float> t;
	while (s.size())
	{
		string str = s.front();
		if (str == "+")
		{
			float num1 = t.top();
			t.pop();
			float num2 = t.top();
			t.pop();
			t.push(num2 + num1);
		}
		else if (str == "-")
		{
			float num1 = t.top();
			t.pop();
			float num2 = t.top();
			t.pop();
			t.push(num2 - num1);
		}
		else if (str == "*" || str == "X")
		{
			float num1 = t.top();
			t.pop();
			float num2 = t.top();
			t.pop();
			t.push(num2 * num1);
		}
		else if (str == "/")
		{
			float num1 = t.top();
			t.pop();
			float num2 = t.top();
			t.pop();

			if (!num1)
				return -1;

			t.push(num2 / num1);
		}
		else
		{
			t.push(stof(s.front()));
		}
		s.pop();
	}

	return t.top();
}