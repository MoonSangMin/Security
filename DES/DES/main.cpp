#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
using namespace std;
string round_keys[16];
string pt;

//10진수를 2진수로
string convertDecimalToBinary(int decimal)
{
	string binary;
	while (decimal != 0) {
		binary = (decimal % 2 == 0 ? "0" : "1") + binary;
		decimal = decimal / 2;
	}
	while (binary.length() < 4) {
		binary = "0" + binary;
	}
	return binary;
}

//2진수를 10진수로
int convertBinaryToDecimal(string binary)
{
	int decimal = 0;
	int counter = 0;
	int size = binary.length();
	for (int i = size - 1; i >= 0; i--)
	{
		if (binary[i] == '1') {
			decimal += pow(2, counter);
		}
		counter++;
	}
	return decimal;
}

//1번 쉬프트 연산
string shift_left_once(string key_chunk) {
	string shifted = "";
	for (int i = 1; i < 28; i++) {
		shifted += key_chunk[i];
	}
	shifted += key_chunk[0];
	return shifted;
}

//2번 쉬프트 연산
string shift_left_twice(string key_chunk) {
	string shifted = "";
	for (int i = 0; i < 2; i++) {
		for (int j = 1; j < 28; j++) {
			shifted += key_chunk[j];
		}
		shifted += key_chunk[0];
		key_chunk = shifted;
		shifted = "";
	}
	return key_chunk;
}

//XOR 연산을 구현
string Xor(string a, string b) {
	string result = "";
	int size = b.size();
	for (int i = 0; i < size; i++) {
		if (a[i] != b[i]) {
			result += "1";
		}
		else {
			result += "0";
		}
	}
	return result;
}

//아래는 키 생성 알고리즘
void generate_keys(string key) {
	int pc1[56] = {
	57,49,41,33,25,17,9,
	1,58,50,42,34,26,18,
	10,2,59,51,43,35,27,
	19,11,3,60,52,44,36,
	63,55,47,39,31,23,15,
	7,62,54,46,38,30,22,
	14,6,61,53,45,37,29,
	21,13,5,28,20,12,4
	};
	int pc2[48] = {
	14,17,11,24,1,5,
	3,28,15,6,21,10,
	23,19,12,4,26,8,
	16,7,27,20,13,2,
	41,52,31,37,47,55,
	30,40,51,45,33,48,
	44,49,39,56,34,53,
	46,42,50,36,29,32
	};
	// PC1을 거쳐 압축 과정을 거친다.
	string perm_key = "";
	for (int i = 0; i < 56; i++) {
		perm_key += key[pc1[i] - 1];
	}
	// L과 R로 나눈다
	string left = perm_key.substr(0, 28);
	string right = perm_key.substr(28, 28);
	for (int i = 0; i < 16; i++) {
		// 1, 2, 9, 16라운드에서는 1번만 쉬프트 연산을 하고
		if (i == 0 || i == 1 || i == 8 || i == 15) {
			left = shift_left_once(left);
			right = shift_left_once(right);
		}
		// 나머지 라운드에서는 2번 쉬프트 연산을 한다.
		else {
			left = shift_left_twice(left);
			right = shift_left_twice(right);
		}
		// L과 R을 합친다.
		string combined_key = left + right;
		string round_key = "";
		// PC2를 거쳐 라운드 키를 생성한다.
		for (int i = 0; i < 48; i++) {
			round_key += combined_key[pc2[i] - 1];
		}
		round_keys[i] = round_key;
	}

}

//아래는 DES 알고리즘의 구현
string DES() {
	int initial_permutation[64] = {
	58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6,
	64,56,48,40,32,24,16,8,
	57,49,41,33,25,17,9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7
	};
	int expansion_table[48] = {
	32,1,2,3,4,5,4,5,
	6,7,8,9,8,9,10,11,
	12,13,12,13,14,15,16,17,
	16,17,18,19,20,21,20,21,
	22,23,24,25,24,25,26,27,
	28,29,28,29,30,31,32,1
	};
	int substition_boxes[8][4][16] =
	{ {
		14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
		0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
		4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
		15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
	},
	{
		15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
		3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
		0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
		13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
	},
	{
		10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
		13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
		13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
		1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
	},
	{
		7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
		13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
		10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
		3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
	},
	{
		2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
		14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
		4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
		11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
	},
	{
		12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
		10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
		9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
		4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
	},
	{
		4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
		13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
		1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
		6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
	},
	{
		13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
		1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
		7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
		2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
	} };
	int permutation_tab[32] = {
	16,7,20,21,29,12,28,17,
	1,15,23,26,5,18,31,10,
	2,8,24,14,32,27,3,9,
	19,13,30,6,22,11,4,25
	};
	int inverse_permutation[64] = {
	40,8,48,16,56,24,64,32,
	39,7,47,15,55,23,63,31,
	38,6,46,14,54,22,62,30,
	37,5,45,13,53,21,61,29,
	36,4,44,12,52,20,60,28,
	35,3,43,11,51,19,59,27,
	34,2,42,10,50,18,58,26,
	33,1,41,9,49,17,57,25
	};

	//IP를 적용
	string perm = "";
	for (int i = 0; i < 64; i++) {
		perm += pt[initial_permutation[i] - 1];
	}

	//L과 R로 나눔
	string left = perm.substr(0, 32);
	string right = perm.substr(32, 32);

	//각 라운드를 거침
	for (int i = 0; i < 16; i++) {
		string right_expanded = "";
		//R은 expansion을 거치고
		for (int i = 0; i < 48; i++) {
			right_expanded += right[expansion_table[i] - 1];
		};
		//R과 라운드키를 XOR함
		string xored = Xor(round_keys[i], right_expanded);
		string res = "";

		//S-box를 거쳐 6*8bits -> 4*8bits가 됨
		for (int i = 0; i < 8; i++) {
			string row1 = xored.substr(i * 6, 1) + xored.substr(i * 6 + 5, 1);
			int row = convertBinaryToDecimal(row1);
			string col1 = xored.substr(i * 6 + 1, 1) + xored.substr(i * 6 + 2, 1) + xored.substr(i * 6 + 3, 1) + xored.substr(i * 6 + 4, 1);;
			int col = convertBinaryToDecimal(col1);
			int val = substition_boxes[i][row][col];
			res += convertDecimalToBinary(val);
		}
		// Permutation 적용
		string perm2 = "";
		for (int i = 0; i < 32; i++) {
			perm2 += res[permutation_tab[i] - 1];
		}
		
		//L, R블록 생성
		xored = Xor(perm2, left);
		left = xored;
		if (i < 15) {
			string temp = right;
			right = xored;
			left = temp;
		}
	}
	//L+R
	string combined_text = left + right;
	string ciphertext = "";
	// Inverse IP 적용
	for (int i = 0; i < 64; i++) {
		ciphertext += combined_text[inverse_permutation[i] - 1];
	}

	return ciphertext;
}

string thrbits[8] = {
	"000", "001", "010", "011", "100", "101", "110", "111"
};

int main() {
/*	//64bit 키
	string key = "1010101010111011000010010001100000100111001101101100110011011101";
	//64bit 평문 블록
	pt = "1010101111001101111001101010101111001101000100110010010100110110";
	string apt = pt;
	//키 생성
	generate_keys(key);
	cout << "Plain text: " << pt << endl;
	//암호문 생성 */
	//string ct = DES();
	string ct = "0011110100000101010001101011110000001000011100011010101011111011";
	string ct2 = "0101101100101110010011011110111110001010001111110111001111011011";
	FILE* f = fopen("bruteforce.txt", "w");
	for (int T = 0; T < 8; T++) {
		string key = "0010001000100000000110000100000000000110010100000000001101110000";
		key.replace(12, 3, thrbits[T]);
		for (int U = 0; U < 8; U++) {
			key.replace(28, 3, thrbits[U]);
			for (int V = 0; V < 8; V++) {
				key.replace(44, 3, thrbits[V]);
				for (int S = 0; S < 8; S++) {
					key.replace(60, 3, thrbits[S]);
					generate_keys(key);
					//복호화를 위해 라운드 키를 역으로
					int i = 15;
					int j = 0;
					while (i > j)
					{
						string temp = round_keys[i];
						round_keys[i] = round_keys[j];
						round_keys[j] = temp;
						i--;
						j++;
					}
					pt = ct;
					string decrypted = DES();
					pt = ct2;
					string decrypted2 = DES();
					string ans;
					for (int i = 0; i < 8; i++) {
						if (convertBinaryToDecimal(decrypted.substr(i * 8, 8)) == 0 || ((convertBinaryToDecimal(decrypted.substr(i * 8, 8)) >= 32 && convertBinaryToDecimal(decrypted.substr(i * 8, 8)) < 127))) {
							ans+=(char)convertBinaryToDecimal(decrypted.substr(i * 8, 8));
						}
					}
					for (int i = 0; i < 8; i++) {
						if (convertBinaryToDecimal(decrypted2.substr(i * 8, 8)) == 0 || ((convertBinaryToDecimal(decrypted2.substr(i * 8, 8)) >= 32 && convertBinaryToDecimal(decrypted2.substr(i * 8, 8)) < 127))) {
							ans+=(char)convertBinaryToDecimal(decrypted2.substr(i * 8, 8));
						}
					}
					if (ans.size() == 16) {
						fprintf(f, "키 : %s\n", key.c_str());
						fprintf(f, "이진수 : %s%s\n", decrypted.c_str(), decrypted2.c_str());
						fprintf(f, "%s\n", ans.c_str());
					}
				}
			}
		}
	}
}