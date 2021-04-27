#include <stdio.h>
#include <string.h>
#include <time.h>
#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4.1,sse4.2,avx,popcnt,tune=native")
#define N 256
#define M 128
#define K 64
#define skill_cnt 40

#define swap(x, y)         \
	{                      \
		const int tmp = x; \
		x = y;             \
		y = tmp;           \
	}

typedef unsigned long long u64_t;
typedef unsigned char u8_t;
const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz<>";

void sort10(int *d) {
#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) < (y) ? (y) : (x))
#define _(x, y)                        \
	{                                  \
		const int a = min(d[x], d[y]); \
		const int b = max(d[x], d[y]); \
		d[x] = a, d[y] = b;            \
	}

	_(0, 5) _(1, 6) _(3, 8) _(4, 9);
	_(0, 3) _(2, 7) _(5, 8) _(6, 9);
	_(0, 2) _(1, 4) _(3, 6) _(7, 9);
	_(0, 1) _(2, 4) _(5, 7) _(8, 9);
	_(1, 2) _(3, 5) _(4, 6) _(7, 8);
	_(1, 3) _(2, 5) _(4, 7) _(6, 8);
	_(2, 3) _(4, 5) _(6, 7);
	_(3, 4) _(5, 6);

#undef _
}

const int val_base[N] = {
	0,	 190, 226, 55,	197, 245, 250, 26,	253, 17,  213, 108, 43,	 198, 12,  91,
	166, 225, 251, 47,	13,	 7,	  46,  45,	152, 53,  182, 37,	180, 68,  98,  241,
	128, 170, 22,  222, 123, 140, 204, 88,	142, 179, 100, 80,	237, 124, 171, 1,
	77,	 31,  122, 157, 24,	 191, 103, 76,	167, 218, 242, 160, 48,	 82,  255, 181,
	74,	 8,	  23,  99,	44,	 39,  229, 156, 83,	 172, 194, 211, 92,	 38,  62,  106,
	57,	 96,  9,   25,	109, 127, 65,  201, 148, 151, 135, 107, 97,	 240, 116, 34,
	27,	 233, 110, 220, 162, 177, 145, 246, 15,	 121, 79,  138, 238, 239, 56,  216,
	93,	 20,  174, 70,	63,	 195, 133, 71,	183, 223, 40,  205, 64,	 10,  163, 2,
	188, 187, 36,  112, 52,	 149, 208, 219, 207, 104, 155, 153, 51,	 227, 32,  249,
	196, 193, 175, 221, 252, 115, 185, 42,	199, 87,  200, 130, 248, 230, 49,  30,
	19,	 203, 146, 129, 59,	 94,  60,  58,	154, 228, 73,  159, 105, 119, 111, 136,
	234, 120, 6,   66,	215, 254, 75,  50,	54,	 189, 169, 139, 84,	 61,  224, 161,
	168, 29,  184, 247, 137, 212, 86,  33,	81,	 18,  232, 173, 165, 67,  132, 192,
	147, 90,  14,  28,	231, 89,  3,   85,	176, 236, 217, 164, 144, 95,  131, 126,
	4,	 118, 102, 158, 78,	 235, 117, 202, 113, 134, 35,  178, 69,	 243, 150, 206,
	101, 5,	  72,  214, 186, 16,  209, 143, 125, 11,  114, 21,	41,	 210, 244, 141};
int name_base[M], freq[16], skill[skill_cnt], st[8];
u8_t p, q, s, name[17] = "\0归零--";
int val[N], vtmp[N];
int q_len;
inline u8_t gen() {
	q += val[++p];
	swap(val[p], val[q]);
	u8_t u = val[(val[p] + val[q]) & 255];
	q += val[++p];
	swap(val[p], val[q]);
	return (u << 8 | val[(val[p] + val[q]) & 255]) % skill_cnt;
}
int load_name() {
	q_len = -1;
	memcpy(val, val_base, sizeof val);
	for (int _ = 0; _ < 2; _++)
		for (int i = s = 0; i < N; i++) {
			(i & 15) && (s += name[i & 15]);
			s += val[i];
			swap(val[i], val[s]);
		}
#define WORK(x) \
	if ((val[i + x] * 181 + 199) & 128) name_base[++q_len] = (val[i + x] * 53) & 63 ^ 32;
	int i = 0;
	for (; i < N && q_len < 30; i += 8) {
		WORK(0) WORK(1) WORK(2) WORK(3) WORK(4) WORK(5) WORK(6) WORK(7)
	}
#define median(x, y, z) max(min(x, y), min(max(x, y), z))
#define a name_base
	st[0] = median(a[10], a[11], a[12]);
	st[1] = median(a[13], a[14], a[15]);
	st[2] = median(a[16], a[17], a[18]);
	st[3] = median(a[19], a[20], a[21]);
	st[4] = median(a[22], a[23], a[24]);
	st[5] = median(a[25], a[26], a[27]);
	st[6] = median(a[28], a[29], a[30]);
	int V = st[0] + st[1] + st[2] + st[3] + st[4] + st[5] + st[6];
	if (V < 307) return 0;
	sort10(a);
	st[7] = 154 + a[3] + a[4] + a[5] + a[6];
#undef a
	if (V * 3 + st[7] < 1320) return 0;
	for (; i < N; i += 8) {
		WORK(0) WORK(1) WORK(2) WORK(3) WORK(4) WORK(5) WORK(6) WORK(7)
	}
	return 1;
}

void calc_skills() {
	int *a = name_base + K;
	for (int i = 0; i < skill_cnt; i++) skill[i] = i;
	p = q = s = 0;
	for (int _ = 0; _ < 2; _++)
		for (int i = 0; i < skill_cnt; i++) {
			s = (s + gen() + skill[i]) % skill_cnt;
			swap(skill[i], skill[s]);
		}
	int last = -1;
	for (int i = 0, j = 0; j < 16; i += 4, j++) {
		int p = min(min(a[i], a[i + 1]), min(a[i + 2], a[i + 3]));
		if (p > 10 && skill[j] < 35) {
			freq[j] = p - 10;
			if (skill[j] < 25) last = j;
		} else
			freq[j] = 0;
	}
	if (last != -1) freq[last] <<= 1;
	if (freq[14] && last != 14)
		freq[14] += min(min(name_base[60], name_base[61]), freq[14]);
	if (freq[15] && last != 15)
		freq[15] += min(min(name_base[62], name_base[63]), freq[15]);
}

int n;
long long l, r;
char fname[N];
int idx[16];
int main(int argc, char **argv) {
	sscanf(argv[1], "%lld", &l);
	sscanf(argv[2], "%lld", &r);
	for (u64_t i = l, j = 1; i < r; i++, j++) {
		name[9] = charset[(i >> 36) & 63];
		name[10] = charset[(i >> 30) & 63];
		name[11] = charset[(i >> 24) & 63];
		name[12] = charset[(i >> 18) & 63];
		name[13] = charset[(i >> 12) & 63];
		name[14] = charset[(i >> 6) & 63];
		name[15] = charset[(i >> 0) & 63];
		if (load_name()) {
			calc_skills();
			int V = 0;
			for (int j = 0; j < 7; j++) V += st[j] += 36;
			int flag = 0, flag2 = 0, flag3 = 0;
			int sum = V + st[7] / 3, sum2 = 0;
			for (int _ = 0; _ < 16; _++) sum2 += freq[_];
			// sum: 八围 ; sum2: 技能熟练度总和 ; freq: 每项技能熟练度
			if (sum >= 712) flag = 1;  // 八围达到四表标准
			if (st[4] <= 55 || sum >= 707 && st[4] <= 60) flag = 1;	 // 魔低号
			if (sum - st[0] >= 645 && st[4] >= 85 && st[6] >= 85)
				flag = 1;  // 攻低号, 要求 魔/智 都不能低
			for (int _ = 0; _ < 16; _++) {
				if (freq[_] >= 70) flag3 = 1;
				if (freq[_] >= 80) flag = flag2 = 1;  // 单项技能超高号
				if (skill[_] == 21)					  // 背刺
					if (st[0] + st[4] + 10 < st[1] + st[5] && freq[_] > 50)
						flag = flag2 = 1;
				// 熟练度 > 50 且 攻击属性 < 防御属性
				if (skill[_] == 17)						 // 净化
					if (freq[_] > 65) flag = flag2 = 1;	 // 熟练度 > 65
			}
			if (sum2 >= 160) flag = 1;	// 技能总熟练度 >= 160
			if (sum2 <= 120 && !flag2 && sum <= 722)
				flag = 0;  // 去除技能较少且没有单项好技能的低八围号
			if (sum2 <= 100 && !flag2 && sum <= 732)
				flag = 0;  // 去除技能很少且没有单项好技能的中低八围号
			if (flag) {
				fputs((char *)name + 1, stdout);
				puts(flag3 ? "@powerless+" : "@powerless");
			}
		}
		if (j == 2e6) {
			j -= 2e6;
			fputs("\n", stderr);
		}
	}
	return 0;
}