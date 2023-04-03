#include <iostream>
#include <vector>
#include <cstdlib>
#include <ap.hpp>
#include <windows.h>
using namespace std;



class RSA {
private:
    ap_int<2048> p;
    ap_int<2048> q;
    ap_int<4096> n;
    ap_int<4096> phi;
    ap_int<2048> ex;
    ap_int<4096> d;
    string messange;
    string enmessange;
    string demessange;
    vector <ap_int <4096>> NumMes;
    vector <ap_int <4096>> EnNumMes;
    vector <ap_int <4096>> DeNumMes;
    
public: 
    RSA(unsigned int x0, unsigned int c, unsigned int t, string messange) {
        this->messange = messange;
        searchPrimNumbers(x0, c, t);
        n = p * q;
        phi = (p - 1) * (q - 1);
        openex();
        d = mod_inverse(ex, phi);
        
    }
    RSA(ap_int<4096> d, ap_int<4096> n) {
        this->d = d;
        this->n = n;
        scanEnNumMes();

    }
    bool is_prime_fermat(ap_int<4096>, int) ;
    ap_int<2048>poww(ap_int <2048>, ap_int <2048>);
    ap_int<4096> power(ap_int<4096>, ap_int<4096>, ap_int<4096>);
    void searchPrimNumbers(const unsigned int&, const unsigned int&, const short int&);
    void openex();
    ap_int<4096> extended_gcd(ap_int<4096>, ap_int<4096>, ap_int<4096>&, ap_int<4096>&);
    ap_int<4096> mod_inverse(ap_int<4096>, ap_int<4096>);
    void encryption();
    void decoding();
    void scanEnNumMes();

    void wordToNum();
    void numToWord();

    void printEncryption();
    void printDecoding();
    void printDemessange(); 
   
    
    ap_int <4096> getPrivateEx();
    ap_int <4096> getN();
};

ap_int<4096> RSA::power(ap_int<4096> base, ap_int<4096> exponent, ap_int<4096> modulo) {
    ap_int<4096> V = exponent, S = 1, c = base, n = modulo;
    while (V > 0) {
        if (V % 2 == 1) {
            S = (S * c) % n;
            V = (V - 1) / 2;
        }
        else V = V / 2;
        c = (c * c) % n;

    }

    return S;
}

ap_int<2048> RSA::poww(ap_int <2048> base, ap_int <2048> ex) {
    ap_int <2048> answer = 1;
    while (ex) {
        if (ex & 1) answer *= base;
        base *= base;
        ex >>= 1;
    }
    return answer;
}

bool RSA:: is_prime_fermat(ap_int<4096> n, int k) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 0; i < k; i++) {
        ap_int<4096> a = rand() % (n - 2) + 2;
        if (power(a, n - 1, n) != 1) return false;
    }
    return true;
}

void RSA ::searchPrimNumbers(const unsigned int& x0, const unsigned int& C, const short int& t) {
    int L;
    ap_int <2048> y0 = x0, c = C;
    vector <int> T(0);
    int s, i = 0;
    T.push_back(t);
    while (T[i] >= 17) {
        if (T[i] >= 17) {
            T.push_back(T[i] / 2); // шаг 2
            i++;

        }

    }
    s = i + 1;
    //cout << s << " ";
    srand(time(NULL));
    ap_int<2048>py; // шаг 3
    for (unsigned long long i = pow(2, T[s - 1] - 1); i < pow(2, T[s - 1] + 1); ++i) {
        if (is_prime_fermat(i, 3)) {
            py = i;
            break;
        }
    }
    vector <ap_int<2048>> PY(s);
    PY[s - 1] = py;
    int m = s - 2; // шаг 4
    L = s - 2;

m5: int r = T[m + 1] / 16; // шаг 5
m6:
    vector <ap_int<2048>> Y(r);
    if (r)Y[0] = (19381 * y0 + c) % 65536;
    else Y.push_back((19381 * y0 + c) % 65536);

    long long unsigned pw2 = pow(2, 16 * r);

    ap_int<2048> YY = Y[0] * pw2;
    for (int i = 1; i < r; i++) {
        Y[i] = (19381 * Y[i - 1] + c) % 65536; //шаг 6


        if (i != r - 1)YY += Y[i] * pw2; // шаг 7
    }
    if (r)y0 = Y[r - 1]; // шаг 8
    else y0 = Y[0];

    for (int i = s - 2; i >= 0; i--) {
        ap_int<2048> N = ((poww(2, T[m + 1] - 1) / PY[i + 1]) + ((poww(2, T[m + 1] - 1) * YY) / (PY[i + 1] * pw2))); // шаг 9??? (poww(2,t-1) / q) + (poww(2, t - 1) / (2*t));
        if (N % 2) N++;
        unsigned int k = 0;
    m11:    ap_int<2048> P = PY[i + 1] * (N + k) + 1;
        if (P > poww(2, T[i])) {
            L = i;
            goto m6;
        }
        if ((power(2, P - 1, P) == 1) && (power(2, N + k, P) != 1)) {
            m -= 1;
            PY[i] = P;
            /*if (i == s - 2) cout << PY[i + 1] << "     " << T[i + 1] << " " << YY << "\n";
            cout << PY[i] << "     " << T[i] << " " << YY << "\n";*/
        }
        else {
            k += 2;
            goto m11;
        }
    }
    ap_int<2048> Prime[2];
    p = PY[0];
    q = PY[1];
    
    

}

void RSA::openex() {
    
    ap_int<4096> P = 0;
    if (phi % 2) P = phi;
    else P = phi - 1;
    for (ap_int<4096> i = P; i > 1; i-=2) {
        if (is_prime_fermat(i, 2) && phi % i) {
            ex = i;
            break;
        }
    }
}
ap_int<4096> RSA:: extended_gcd(ap_int<4096> a, ap_int<4096> b, ap_int<4096>& x, ap_int<4096>& y) {
    if (a == 0) {
        x = 0; y = 1;
        return b;
    }
    ap_int<4096> x1, y1;
    ap_int<4096> gcd = extended_gcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return gcd;
}
ap_int<4096> RSA ::mod_inverse(ap_int<4096> a, ap_int<4096> m) {
    ap_int<4096> x, y;
    ap_int<4096> gcd = extended_gcd(a, m, x, y);
    if (gcd != 1) return -1;
    return (x % m + m) % m;
}

void RSA::encryption() {
    wordToNum();
    for (int i = 0; i < NumMes.size(); i++) {
        EnNumMes.push_back(power(NumMes[i], ex, n));
        
    }
}


void RSA::decoding() {
    for (int i = 0; i < EnNumMes.size(); i++) {
        DeNumMes.push_back(power(EnNumMes[i], d, n));
    }
    
}

void RSA::wordToNum() {
    ap_int<4096> NumTemp = 0;
    int ASCII;
    for (int i = 0; i < messange.length(); i++) {
        ASCII = int(unsigned char(messange[i]))+100;
         if (NumTemp * 1000 + ASCII < n) NumTemp = NumTemp * 1000 + ASCII;
        else {
            NumMes.push_back(NumTemp);
            NumTemp = ASCII;
        }
         
         
        
    }
    NumMes.push_back(NumTemp);
    for (int i = 0; i < NumMes.size(); i++) {
     
    }
    
}
ap_int <4096> RSA:: getPrivateEx() { return d; }
ap_int <4096> RSA:: getN() { return n; }

void RSA::numToWord() {
    ap_int<4096> NumTemp;
    for (int i = 0; i < DeNumMes.size(); i++) {
        NumTemp = DeNumMes[i];
        int l = 0;
        for (ap_int<4096> j = 1; j < NumTemp; j *= 1000) l++;
        while (NumTemp) {
            
            if (NumTemp > 1000) {
                    demessange.push_back(char(NumTemp / poww(1000, l-1)-100));
            }
            else {
                demessange.push_back(char(NumTemp-100));
            }
            NumTemp %= poww(1000, --l);
           
        }
    }
} 

void RSA::scanEnNumMes() {
    string Num, El = "";
    
    getline(cin, Num);
    getline(cin, Num);
    //cout << Num;
    bool F = false;
    for (int i = 0; i < Num.size(); i++) {
        if (Num[i] != ' ') El += Num[i];
        else {
            ap_int<4096> TempElem;
            TempElem= El;
            EnNumMes.push_back(TempElem);
            El = "";
        }
    }
    ap_int<4096> TempElem;
    TempElem = El;
    EnNumMes.push_back(TempElem);
    El = "";
    
}
void RSA::printEncryption() {
    for (int i = 0; i < EnNumMes.size(); i++) {
        cout << EnNumMes[i] << " ";
    }
    cout << "\n";
}
void RSA::printDecoding() {

    for (int i = 0; i < DeNumMes.size(); i++) {
        cout << DeNumMes[i] << " ";
    }
    cout << "\n";
}
void RSA::printDemessange() {
    numToWord();
    for (int i = 0; i < demessange.size(); i++)
        cout << demessange[i];
    cout << '\n';
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    //setlocale(LC_ALL, "Russian");
    int x0, c, length, type = 1;
    string messange;

        cout << "Выберите действие:\n1) Зашифровать сообщение;\n2) Расшифровать сообщение;\n3) Выход.\n";
        cin >> type;
        switch (type) 
        {
        case 1: {
            cout << "Для шифрования введите следующие параметры: х, с, length. Где х и с - любые числа от 0 до 65536, с - нечетное число, length - число от 17 до 1900, чем больше тем сильнее уровень защиты\n";
            cout << "x = "; cin >> x0;
            cout << "c = "; cin >> c;
            cout << "length = "; cin >> length;
            system("cls");
            cout << "Введите сообщение: "; getline(cin,messange); getline(cin, messange);
            RSA rsa(x0, c, length, messange);
            rsa.encryption();
            cout << "Зашифрованное сообщение: ";
            rsa.printEncryption();
            cout << "private key = {" << rsa.getPrivateEx() << ", " << rsa.getN() << "}\n";
        } break;
        case 2: {
            ap_int <4096> enmessange, n, d;
            cout << "Введите две части секретного ключа:\nПервая часть: "; cin >> d;
            cout << "Вторая часть: "; cin >> n;

            cout << "Зашифрованное сообщение: ";
            RSA rsa(d, n);
            rsa.decoding();
            rsa.printDemessange();
        } break;
        default: return 0;
            break; 
        }
    
    
        return 0;
}
