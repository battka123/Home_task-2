#include <iostream>
#include <random>
#include <cstdint>
#include <string>
#include <fstream>

using namespace std;
// Длина блока 4 байта
uint32_t inleft(uint32_t in, uint32_t n=4) // Сдвиг влево на 1 бита n - размер блока
{
    return (in << 1) | (in >> (n*8-1));
}

uint32_t inright(uint32_t in, uint32_t n=4) // Сдвиг вправо на 1 бита n - размер блока
{
    return (in >> 1) | (in << (n*8-1));
}

void shifr_deshifr(const char * in, char *out, int len, int key, bool shifr=true)
{
    int len1=len/4; // Число полных блоков по 2 байта
    int len2=len % 4; // Размер неполного последнего блока
    mt19937 bas(key);
    uniform_int_distribution<uint32_t> gen(0, 0xffffffff);
    uint32_t rez, gamma; //Переменные шифрованного символа и гаммы
    if (len2) len1++; // В цикле последним будет не полный блок
    int size = 4;
    for(int i=0; i < len1; i++) // Перебираем блоки по 4 байта (кроме последнего)
    {
        if (len2) if (i==len1-1) { // Для последнего блока
                size=len2; // размер последнего блока
                rez=0; // Последний блок байты не используемые равны 0
            }
        memcpy(&rez, in + i*4, size); //Копируем блок 4 байта в rez
        if (!shifr) //При расшифровке
            rez = inright(rez);
        gamma=gen(bas); //Создаем гамму
        if (len2) if (i==len1-1) { // Для последнего не полного блока обнуляем не используемые байты гаммы
                uint32_t maska=0xff; // Последний байт заполнен единицами
                for(uint8_t i=1; i<len2; i++)
                {
                    maska<<=8; maska+=0xff;
                }
                gamma&=maska; // Обнуляем не используемые байты
            }
        rez ^= gamma; //Накладываем гамму на блок
        if (shifr) //При шифровке
            rez = inleft(rez);
        memcpy(out + i*4, &rez, size); //Копируем то, что в rez, в выходной массив out
    }
}

int main(int arqc, char *arqv[]) {
    setlocale(LC_ALL, "RUS");
    string mode = arqv[1]; //Режим работы
    string file_name = arqv[2]; //Файл
    if (mode == "encryption") { //Если режим шифрования
        string in_str; //Входная строка
        int key; //Ключ
        cout<<"Enter your message: ";
        getline(cin, in_str);
        cout<<"Enter your new_key: ";
        cin>>key; //Ключ шифрования
        char *out_str = new char[in_str.size()];
        shifr_deshifr(in_str.c_str(), out_str, in_str.size(), key); //Запускаем функцию для шифрования
        ofstream fout(file_name, ios_base::out);
        cout<<"Encryption message "<<out_str; //Вывод зашифрованного сообщения в файл и консоль
        fout<<out_str;
        fout.close();
        delete[] out_str;
    } if (mode == "decryption"){ //Для режима расшифрования
        ifstream fin(file_name, ios_base::in);
        string in_str;
        string bufer;
        int key;
        cout<<"Enter your key: ";
        cin>>key;
        while(getline(fin,bufer)){
            in_str+=bufer+'\n'; //Считываем из файла зашифрованную строку
        }
        in_str.erase(in_str.end()-1);
        char *out_str = new char[in_str.size()];
        shifr_deshifr(in_str.c_str(), out_str, in_str.size(), key, false);
        cout<<"Decryption message: ";
        for (int i=0; i<in_str.size(); ++i){
            cout<<out_str[i]; //Вывод расшифрованной строки
        }
        cout<<endl;
        fin.close();
        delete[] out_str;
    }
    return 0;
}
