#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#define boyut 8

string tahta[boyut][boyut];

//yukarıdaki boyut değişirse sayı/harf ekleyip çıkart (burayı fonksiyon olarak düzenle bir ara)
char yatay_aks[] =  { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
int düşey_aks[] = {8, 7, 6, 5, 4, 3, 2, 1};

typedef struct
{
    bool yeri[boyut][boyut];
}
taş;

const int vezir_sayısı = 8;
taş vezir[vezir_sayısı];

void taslari_sor_yerlestir(void);
void taşları_yatay_dağıt(void);
void tahtayı_yerleştir(void);
void tahtayı_görüntüle(void);
void aşağı(int vezirNO);
void yukarı(int vezirNO);
int sayı(string yer);

bool yatay_kontrol(int vezirNO);
bool çapraz_kontrol(int vezirNO);
bool kare_dolu(int i, int j);

int main(void)
{
    //vezir yerlerini kullanıcıdan al ve tahtaya yerleştir. sonra tahtayı görsel olarak print et.
    printf("\n");
    taslari_sor_yerlestir();
    printf("\n");
    printf("ilk yerleşim:\n");
    tahtayı_görüntüle();

    //vezirleri yatay eksende her sütuna bir tane gelecek şekide dağıt.
    taşları_yatay_dağıt();

    //tüm vezirler çakışmadan sıralanana kadar döngüye sok.
    bool taşlar_çakışıyor = true;
    while(taşlar_çakışıyor)
    {
        int sayaç = 0;
        for (int i = 0; i < vezir_sayısı; i++)
        {
            int hamle = 0;
            while (!çapraz_kontrol(i) || !yatay_kontrol(i))
            {
                yukarı(i);
                hamle++;
                sayaç = hamle;
                if (hamle == boyut)
                {
                    aşağı(0);
                    break;
                }
            }
            if (hamle == boyut)
            {
                break;
            }
        }
        if(sayaç < boyut)
        {
        taşlar_çakışıyor = false;
        }
    }
    //vezirlerin son dizilmiş halini print et.
    printf("son yerleşim:\n");
    tahtayı_görüntüle();
}

//kullanıcıdan taş yerlerini alır ve görsele uyumlu yerleştirir.
void taslari_sor_yerlestir(void)
{
    tahtayı_yerleştir();
    int i = 0;
    int no;
    string yer;
    printf("taşların yerini giriniz: (örn: e4 veya E4)\n");
    do
    {
        yer = get_string("%i. vezir: ", i + 1);
        no = sayı(yer);
        while ((strlen(yer) < 2 || strlen(yer) > 3) || (int)toupper(yer[0]) % 65 >= boyut || (int)toupper(yer[0]) %65 < 0 || no <= 0 ||
               no > boyut || kare_dolu(düşey_aks[no], (int)toupper(yer[0]) % 65 ))
        {
            printf("hatalı giriş! tekrar giriniz:\n");
            yer = get_string("%i. vezir: ", i + 1);
            no = sayı(yer);
        }
        vezir[i].yeri[düşey_aks[no]][(int)toupper(yer[0]) % 65] = true;
        tahtayı_yerleştir();
        i++;
     }
     while(i < vezir_sayısı);
}

//taşları her sütuna bir tane gelecek şekilde dizer. satır yüksekliğini değiştirmez.
void taşları_yatay_dağıt(void)
{
    for (int k = 0; k < vezir_sayısı; k++)
    {
        for (int i = 0; i < boyut; i++)
        {
            for (int j = 0; j < boyut; j++)
            {
                if (vezir[k].yeri[i][j] == true)
                {
                    vezir[k].yeri[i][j] = false;
                    vezir[k].yeri[i][k] = true;
                    tahtayı_yerleştir();
                }
            }
        }
    }
}

//hamle veya dizilimden sonra tahtaya hangi taşın nerede olduğunu kaydeder.
void tahtayı_yerleştir(void)
{
    for (int i = 0; i < boyut; i++)
    {
        for (int j = 0; j < boyut; j++)
        {
            //buraya vezirleri yerleştir.
            for (int k = 0; k < vezir_sayısı; k++)
            {
                if (vezir[k].yeri[i][j] == true)
                {   //vezir varsa dolu kare
                    tahta[i][j] = "_O|";
                    break;
                }
                else
                {  //vezir yoksa boş kare
                   tahta[i][j] = "__|";
                }
            }
        }
    }
}

//tahtanın o anki halini komut satırında görsel olarak görüntülemek için.
void tahtayı_görüntüle(void)
{
    printf("    ");
    for (int i = 0; i < boyut; i++)
        {
            printf("__ " );
        }
        printf("\n");
        for (int i = 0; i < boyut; i++)
        {
            for (int j = 0; j < boyut; j++)
            {
                if (j == 0)
                {
                    if(düşey_aks[i] < 10)
                    {
                        printf(" %i |", düşey_aks[i]);
                    }
                    else
                    {
                        printf("%i |", düşey_aks[i]);
                    }
                }
                printf("%s", tahta[i][j]);
            }
            printf("\n");
        }
        printf("   ");
        for (int i = 0; i < boyut; i++)
        {
            printf(" %c ", yatay_aks[i]);
        }
    printf("\n");
    printf("\n");
}

//koordinatları girilen karede taş olup olmadığını kontrol eder.
bool kare_dolu(int i, int j)
{
    if(strcmp(tahta[i][j], "_O|") == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//numarası girilen taş aşağı yönde bir hamle gider.
void aşağı(int vezirNO)
{
    for (int i = 0; i < boyut; i++)
    {
        for (int j = 0; j < boyut; j++)
        {
            if(vezir[vezirNO].yeri[i][j] == true && i != boyut - 1 && !kare_dolu(i + 1, j))
            {
                vezir[vezirNO].yeri[i][j] = false;
                vezir[vezirNO].yeri[i + 1][j] = true;
                tahtayı_yerleştir();
                return;
            }
            else if (vezir[vezirNO].yeri[i][j] == true && i == boyut - 1 && !kare_dolu(0, j))
            {
                vezir[vezirNO].yeri[i][j] = false;
                vezir[vezirNO].yeri[0][j] = true;
                tahtayı_yerleştir();
                return;
            }
        }
    }
}

//numarası girilen taş yukarı yönde bir hamle gider. eğer en yukarıdaysa aşağıdan çıkar.
void yukarı(int vezirNO)
{
    for (int i = 0; i < boyut; i++)
    {
        for (int j = 0; j < boyut; j++)
        {
            if(vezir[vezirNO].yeri[i][j] == true && i != 0 && !kare_dolu(i - 1, j))
            {
                vezir[vezirNO].yeri[i][j] = false;
                vezir[vezirNO].yeri[i - 1][j] = true;
                tahtayı_yerleştir();
                return;
            }
            else if (vezir[vezirNO].yeri[i][j] == true && i == 0 && !kare_dolu(boyut - 1, j))
            {
                vezir[vezirNO].yeri[i][j] = false;
                vezir[vezirNO].yeri[boyut - 1][j] = true;
                tahtayı_yerleştir();
                return;
            }
        }
    }
}

// taşın satırında solda kalan kısım için çakışan başka taşı kontrol eder.
bool yatay_kontrol(int vezirNO)
{
    for (int i = 0; i < boyut; i++)
    {
        for (int j = 0; j < boyut; j++)
        {
            if(vezir[vezirNO].yeri[i][j] == true)
            {
                int m = 0;
                for (int k = 0; k < j + 1; k++)
                {
                    if (kare_dolu(i, k))
                    {
                        m++;
                    }
                }
                if(m == 1)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }
    return false;
}

// taşın sol tarafı için çapraz yollarında çakışan başka taşı kontrol eder.
bool çapraz_kontrol(int vezirNO)
{
    for (int i = 0; i < boyut; i++)
    {
        for (int j = 0; j < boyut; j++)
        {
            if(vezir[vezirNO].yeri[i][j] == true)
            {
                int m = 0;
                for (int k = j, n = i; k >= 0 && n < boyut; k--, n++)
                {
                    if (kare_dolu(n, k))
                    {
                        m++;
                    }
                }
                for (int k = j, n = i; k >= 0 && n >= 0; k--, n--)
                {
                    if (kare_dolu(n, k))
                    {
                        m++;
                    }
                }
                if(m == 2)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }
    return false;
}
//taşları girerken koordinatın harften sonraki sayılarını alır stringden int'e çevirir.
int sayı(string yer)
{
    int sayı = 0;
    if (strlen(yer) == 2)
    {
        sayı = (int)(yer[1] - 48);
    }
    else if (strlen(yer) == 3)
    {
        sayı = (int)(yer[1] - 48) * 10 + (int)(yer[2] - 48);
    }
    return sayı;
}