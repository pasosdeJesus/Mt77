#include <iostream>
#include <string>
#include <locale>
#include <map>

#define MAXCAD 50

using namespace std;

string wchar_t_a_string(wchar_t wc)
{
    std::mbstate_t state {};
    std::string mb(MB_CUR_MAX, '\0');
    std::wcrtomb(&mb[0], wc, &state);
    return mb;
}

wstring to_wstring(string &arg)
{
    wchar_t* wchr = new wchar_t(arg.length());
    mbstowcs(wchr, arg.c_str(), arg.length());
    return wstring(wchr);
}

const int tamnoagregan = 40;
string noagregan[tamnoagregan] = {

                                         "Y",  // Conjunciones
                                         "O",
                                         "EL", // Artículos
                                         "LA",
                                         "LOS",
                                         "LAS",
                                         "UN",
                                         "UNA",
                                         "UNOS",
                                         "UNAS",
                                         "A", // Preposiciones http://www.apoyolingua.com/LASPREPOSICIONES.htm
                                         "ANTE",
                                         "BAJO",
                                         "CON",
                                         "CONTRA",
                                         "DE",
                                         "DESDE",
                                         "DURANTE",
                                         "EN",
                                         "ENTRE",
                                         "HACIA",
                                         "HASTA",
                                         "MEDIANTE",
                                         "PARA",
                                         "POR",
                                         "SEGUN",
                                         "SIN",
                                         "SOBRE",
                                         "TRAS",
                                         "QUE", //Otros
                                         "LE",
                                         "LES",
                                         "DEL",
                                         "AL",
                                         "CUANDO",
                                         "SU",
                                         "SUS",
                                         "COMO",
                                         "O",
                                         "MAS"
                                 };

// using convert_t = codecvt_utf8<wchar_t>;
// wstring_convert<convert_t, wchar_t> strconverter;
//
// string to_string(wstring wstr)
// {
//     return strconverter.to_bytes(wstr);
// }
//
// wstring to_wstring(string str)
// {
//     return strconverter.from_bytes(str);
// }

map<wchar_t, string> normCaracteres{
    {L'Á', "A"},
    {L'É', "E"},
    {L'Í', "I"},
    {L'Ó', "O"},
    {L'Ú', "U"},
    {L'Ü', "U"},
    {L'Ñ', "N"},
};

string normalizaCaracter(wchar_t c)
{
    c = towupper(c);
    // wcout << c << "  " << (int) c << endl;

    string o = "";

    if(normCaracteres[c] != "")
    {
        o += normCaracteres[c];
    }
    else if ( c > 127 )
    {
        o += wchar_t_a_string(c);
    }
    else
    {
        o += (int)c;
    }

    return o;
}

string normaliza(string s)
{
    wstring wstr = to_wstring(s);

    wstring::iterator i;
    string o = "";
    int c;

    for (c = 0, i = wstr.begin(); i != wstr.end() && c <= (int)MAXCAD; c++ , i++) {
        if (*i == '.' && (i+1 != wstr.end())
            && (normalizaCaracter(*(i+1)) != "")) {
            o += ".";
        } else {
            o += normalizaCaracter( *i );
        }
    }

    for (int i = 0; i < tamnoagregan; i++) {
        if (o == noagregan[i]) {
            return string("");
        }
    }

    return o;
}

int main(int argc, char *argv[])
{
    setlocale(LC_CTYPE, "es_CO.UTF-8");

    for(int i = 1 ; i < argc ; i++)
    {
        string arg(argv[i]);

        // wstring wstr = to_wstring(arg);
        string str = normaliza(arg);

        cout << str << endl;

        // for(char c: arg)
        // {
        //     cout << "\t" << c << " : " << (unsigned int)c << endl;
        // }
    }


}
