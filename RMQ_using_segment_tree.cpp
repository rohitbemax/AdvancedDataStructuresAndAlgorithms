//
// beautiful code
//

#include <algorithm>  // min
#include <bitset>
#include <cassert>  // assert
#include <cmath>
#include <csignal>
#include <deque>    // deque
#include <fstream>  // ifstream
#include <functional> // greater
#include <iomanip>  // setw, setfill
#include <iostream>
#include <limits>   // numeric_limits
#include <map>      // map, multimap
#include <numeric>
#include <queue>    // priority_queue (greatest on top, by default, use greater for smallest on top)
#include <set>      // set
#include <sstream>  // stringstream
#include <stack>    // stack
#include <tuple>    // tuple
#include <unordered_map>
#include <unordered_set> // unordered_set
#include <vector>   // vector

using namespace std;


bool ON = 1;
bool OFF = 0;

#ifdef DEBUGG
bool DEBUG_MODE = ON;
bool LOGS = ON;
#else
bool DEBUG_MODE = OFF;
bool LOGS = OFF;
#endif

template <typename Arg, typename... Args>
void debug(Arg&& arg, Args&&... args) {

	if (LOGS) {
		std::ostream& out = std::cout;
		out << std::forward<Arg>(arg);
		using expander = int[];
		(

			void)expander {
			0, (void(out << ' ' << std::forward<Args>(args)), 0)...
		};
	}
}

template <typename T>
ostream & operator<<(ostream &o1, const vector<T> &v1) {
	for (auto it = v1.begin(); it != v1.end(); it++) {
		o1 << setw(4) << *it << " ";
	}
	return o1;
}

template <typename T>
vector <T> range(T N1, T N2) {
	vector<T> numbers(N2 - N1);
	iota(numbers.begin(), numbers.end(), N1);
	return numbers;
}

template <typename T>
vector <T> zero_till(T N) {
	vector<T> numbers(N);
	iota(numbers.begin(), numbers.end(), 0);
	return numbers;
}

template <typename T>
vector <T> one_till(T N) {
	vector<T> numbers(N);
	iota(numbers.begin(), numbers.end(), 1);
	return numbers;
}

// -----------------------------------
// -----------------------------------
// -----------------------------------

// #define PTypeVal short // 1 to 50
//
//#define NTypeValue int // 1 to 10^5
//#define MTypeValue int // 1 to 10^5
//#define XTypeValue int // 1 to 10^9
//#define VTypeValue int // 0 to 10^6
//#define TypeValue unsigned long long // 0 to 10^6
//#define PTypeVal unsigned long long
//#define VTypeVal unsigned short // long long
//#define PTypeVal long long
//#define DTypeVal long long
//#define TTypeVal unsigned long long
//#define LTypeVal unsigned long long
//#define SumOfLTypeVal unsigned long long

// #define PTypeVal unsigned long long
#define NTypeVal int
#define EleTypeVal int
#define EleTypeVeryLargVal unsigned long long
// #define SizeT unsigned int
// -----------------------------------

/*
struct Node
{
	NTypeVal i;
	EleTypeVal d;
	Node *l, *r, *p;	// left, right, parent
};
*/

template <class NType, class EleType>
class seg_tree {

	NType N;
	EleType Ai;
	deque<NType> v1;   // input array, with N+1 elements - element at index 0, is not used

	deque<NType> v_seg;  // v_seg array, with N+1 elements - element at index 0, is not used
	deque<deque<NType>> v_seg2;  // v_seg array, with N+1 elements - element at index 0, is not used

	// unordered_set <EleType> s1;
	// unordered_multimap <NTypeVal, pair<NTypeVal, EleTypeVal>> m1;
	// unordered_map <pair<NTypeVal, NTypeVal >, EleTypeVal> m1;

	// unordered_multimap <NTypeVal, pair<NTypeVal, EleTypeVal>>
	// pair<NTypeVal, EleTypeVal> least_gr_eq;

	//NType   least_gr_eq_ri;
	//EleType least_gr_eq_rmin;

public:

	seg_tree(istream &cin) {
		cin >> N;

		// v1.reserve(N);

		// get arr
		for (NType i = 0; i <= N - 1; i++) {
			cin >> Ai;
			v1.push_back(Ai);
		}

		///////// cout << one_till(N) << endl;
		// cout << v1 << endl;
		// cout << setw(4) << Ai << " ";
	}

	void build_for_seg_tree_RMQ_wrapper() {
		// build seg arr
		v_seg.resize(4 * N);
		fill(v_seg.begin(), v_seg.end(), numeric_limits<EleType>::max());
		build_for_seg_tree_RMQ(0, N - 1, 0);
	}

	// a_low, a_high & a_mid are indexes of array, and   s_i is index of segtree array
	void build_for_seg_tree_RMQ(const NType &a_low, const NType &a_high, const NType &s_i) {

		if (a_low == a_high) {
			v_seg[s_i] = v1[a_low];
			return;
		}

		auto a_mid = (a_low + a_high) / 2;

		// build left and right seg trees
		build_for_seg_tree_RMQ(a_low, a_mid, 2 * s_i + 1);			// s_i * 2 + 1 is left  child (1 initially)
		build_for_seg_tree_RMQ(a_mid + 1, a_high, 2 * s_i + 2);	// s_i * 2 + 2 is right child (2 initially)

		// update parent in seg tree from its children
		v_seg[s_i] = min(v_seg[2 * s_i + 1], v_seg[2 * s_i + 2]);
	}

	void seg_tree_RMQ_wrapper() {

		for (NType ql = 0; ql <= N - 1; ql++) {
			for (NType qh = ql; qh <= N - 1; qh++) {

				auto min = seg_tree_RMQ(ql, qh, 0, N - 1, 0);
				cout << "[" << ql << "," << qh << "] = " << min << endl;
			}
		}
	}

	EleType seg_tree_RMQ(
		const NType &q_low, const NType &q_high,	// query		low high
		const NType &s_low, const NType &s_high,	// seg array	low high
		const NType &s_i							// seg array	position
	) { // rmq

		// full overlap
		// current seg position (range in array) is inside queried range, so return that aggregate
		if (q_low <= s_low && s_high <= q_high) {
			return v_seg[s_i];
		}

		if (					// no overlap
			s_high < q_low ||	// current seg position is outside left of queried range
			q_high < s_low		// current seg position is outside right of queried range
			) {
			return numeric_limits<EleType>::max();
		}

		auto s_mid = (s_low + s_high) / 2;

		// partial overlap, so get min of both
		return min(
			seg_tree_RMQ(q_low, q_high, s_low, s_mid, 2 * s_i + 1),		// s_i * 2 + 1 is left  child (1 initially)
			seg_tree_RMQ(q_low, q_high, s_mid + 1, s_high, 2 * s_i + 2)	// s_i * 2 + 2 is right child (2 initially)
		);

	}

	void doMain() {

		build_for_seg_tree_RMQ_wrapper();
		seg_tree_RMQ_wrapper();

	}
};

// testsss
#define ReturnCountTypeValue char
vector < pair < vector<string>, vector<ReturnCountTypeValue >>> tests = {
	/*	{
			{
				"5",
				"5 10 40 30 28"
			},
			{ 0, 0, 0, 0, 0, 0, 0, 0 } // 4(new) 3 (old)
		}, */
		/*
		*/
	{
					{
						"11",
						"9 3  7 1 8  12 10 20  15 18 5"
					},
					{ 0, 0, 0, 0, 0, 0, 0, 0 } // 4(new) 3 (old)		5*10 = 50
				},
	/*
	{
					{
						"12",
							"9 3  7 1 8  12 12 10 20  15 18 5"
					},
					{ 0, 0, 0, 0, 0, 0, 0, 0 } // 4(new) 3 (old)	6*10 = 50
				},

{
		{
			"5",
			"1 2 3 4 5"
		},
		{ 0, 0, 0, 0, 0, 0, 0, 0 } // 4(new) 3 (old)
	}*/
};

// template <class NType, class VType, class PType, class DType>
template <class NType, class EleType>
class Cls1 {

	//    HNType n;
	//    MType m;
	// deque <pair<VType, NType > > p1;
	//    XType x;
	//    string S;
	//    LenType k;
	//    TType type;
	//    HVType v;
	//    VType v;
	//    PType P;
	//    DType D;
	//    KType K;
	// Heap<NType, pair<LType, TType>> h1;
	// multiset<HVType> se1;
	// deque<HVType> p1;

public:

	Cls1() {
		// LOGS = OFF;
	}


	vector <ReturnCountTypeValue> testFunction(istream & cin) {
		// debug("testFunction - begin\n\n");
		vector <ReturnCountTypeValue> res;
		// --------------------

		// LOGS = 0;


		seg_tree <NType, EleType> o1(cin);
		o1.doMain();


		auto actual_result = 0;
		res.push_back(actual_result);

		return res;
	}

};


int main() {

	if (!DEBUG_MODE) {

		Cls1<NTypeVal, EleTypeVal> o;
		o.testFunction(cin);

		return 0;
	}
	else {

		for (unsigned long i = 0; i < tests.size(); i++) {
			// debug("----------------------- input getting ready ----------------------------- ", "\n");

			auto input = tests[i].first;
			auto expected_output = tests[i].second;

			std::stringstream ss;
			istream &cin = ss;

			for (size_t i = 0; i < input.size(); i++) {
				// debug(input[i], "\n");
				ss << input[i] << endl;
			}

			/*
			ifstream ifs;
			// ifs.open("../lr_input09_dummy.txt");
			ifs.open("../lr_input09.txt");
			string temp;
			vector<string> a;
			getline(ifs, temp); 	ss << temp << endl;
			getline(ifs, temp); 	ss << temp << endl;
			*/


			// debug("----------------------- input ready ----------------------------- ", "\n");

			Cls1<NTypeVal, EleTypeVal> o;
			// Cls1<NTypeVal, VTypeVal, PTypeVal, DTypeVal> o;
			// Cls1<NTypeVal, LTypeVal> o;
			// auto actual_result = o.testFunction(cin, q)[0];
			auto actual_result = o.testFunction(cin)[0];

			//            for (PTypeVal k = 0; k < q; k++) {
			//            Cls1<NTypeVal, LTypeVal, TTypeVal> o;
			//                // // debug("\tactual_result ", actual_result, " ", "expected_output ", expected_output[k], "\n");
			//
			//                // assert(actual_result == expected_output[k]);
			//            }

			// break;

		} // for tests.size()

		return 0;
	}

	return 0;
}
