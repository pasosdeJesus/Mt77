
#include "compresion.hpp"

/*
Inicio
   definir un nodo con caracter, frecuencia, e hijo izquierdo y derecho.
   crear una lista de frecuencia para cada caracter, inicialmente todos estan en 0
   para cada caracter c en la cadena hacer
      incrementar la frecuancia de cada caracter ch en la lista de frecuencias
   terminado

   para cada tipo de caracter hacer
      si la frecuencia de ch no es 0, entonces
         agregar ch y su frecuencia como un nodo a la cola de prioridad Q
   terminado

   mientras Q no este vacio hacer
      eliminar elemento de Q y asignarlo al hijo izquierdo del nodo
      eliminar elemento de Q y asignarlo al hijo derecho del nodo
      pasar al siguiente nodo para encontrar el codigo asignado
   terminado
Fin
*/

void sumarMapas(std::map<char, int> &map1, std::map<char, int> map2) {
    for (std::pair<char, int> p: map2) {
        // si ya existe lo suma, de lo contrario lo crea
        map1[p.first] ? map1[p.first] += p.second : map1[p.first] = p.second ;
    }
}

void imprimirMapa(std::map<char, int> &tendencia) {
    for (std::pair<char, int> v : tendencia) {
        std::cout << v.first << " -> " << v.second << std::endl;
    }
}


nodo_arbol_huffman::nodo_arbol_huffman(char valor, int tendencia) {
    this->valor = valor;
    this->tendencia = tendencia;
    this->hijo_i = nullptr;
    this->hijo_d = nullptr;
}

nodo_arbol_huffman::nodo_arbol_huffman(nodo_arbol_huffman &nah1, nodo_arbol_huffman &nah2) {
    this->valor = 0;
    this->tendencia = nah1.tendencia + nah2.tendencia;
    this->hijo_i = std::make_shared<nodo_arbol_huffman>(nah1);
    this->hijo_d = std::make_shared<nodo_arbol_huffman>(nah2);
}

void nodo_arbol_huffman::prt() {
    std::cout << "'" << this->valor << "' :" << this->tendencia << std::endl;
}

std::string nodo_arbol_huffman::toString() {
    std::stringstream ss;
    ss << "" << this->valor << ":" << this->tendencia;
    return ss.str();
}

Arbol_huffman::Arbol_huffman() {
    this->raiz = nullptr;
}

bool Arbol_huffman::vacio() {
    return this->raiz == nullptr;
}

Arbol_huffman::Arbol_huffman(std::map<char, int> tendencia, std::string archivo) {
    if (!archivo.empty()) {
        sumarMapas(tendencia, Arbol_huffman::cargar(archivo));
    }
    this->construirArbol(tendencia);
}

Arbol_huffman::Arbol_huffman(std::string cadena, std::string archivo) {

    // tendencia de cada caracter
    std::map<char, int> tendencia;

    for (char c : cadena) {
        tendencia[c] ? tendencia[c]++ : tendencia[c] = 1;
    }
    if (!archivo.empty()) {
        sumarMapas(tendencia, Arbol_huffman::cargar(archivo));
    }

    this->construirArbol(tendencia);
}

void Arbol_huffman::construirArbol(std::map<char, int> tendencia) {

    // imprimirMapa(tendencia);

    this->tendencias = tendencia;

    // cola de prioridad invertida, guarda los valores de menor a mayor
    std::priority_queue<nodo_arbol_huffman, std::vector<nodo_arbol_huffman>,
                        std::greater<nodo_arbol_huffman>>
        cp;

    for (std::pair<char, int> v : tendencia) {
        cp.push(nodo_arbol_huffman(v.first, v.second));
    }

    while (cp.size() > 1) {
        nodo_arbol_huffman nah = cp.top();
        cp.pop();
        nodo_arbol_huffman nah2 = cp.top();
        cp.pop();
        cp.push(nodo_arbol_huffman(nah, nah2));
    }

    if (cp.size() != 0) {
        nodo_arbol_huffman nah = cp.top();
        cp.pop();
        this->raiz = std::make_shared<nodo_arbol_huffman>(nah);
    } else {
        this->raiz = nullptr;
    }
}

void Arbol_huffman::conseguirCodigos() {
    _conseguirCodigos(this->raiz, "1");
}

void Arbol_huffman::_conseguirCodigos(std::shared_ptr<nodo_arbol_huffman> nah,
                                      std::string codigo) {
    this->simbolos[nah->valor] = codigo;
    if (nah->hijo_i != NULL) {
        _conseguirCodigos(nah->hijo_i, codigo + "1");
    }
    if (nah->hijo_d != NULL) {
        _conseguirCodigos(nah->hijo_d, codigo + "0");
    }
}

std::string Arbol_huffman::descomprimir(std::string binCodigo) {
    std::string codigo = "";
    // convertir cada caracter a su representacion binaria
    for (char c : binCodigo) {
        std::bitset<8> btc(c);
        codigo += btc.to_string();
    }
    std::string desc = "";
    int posicion = 0;
    // cada caracter empieza con '1', por lo que hay que revisar que
    // no se empiece con '0', ya que eso seria relleno para compretar el byte
    while (codigo.length() > posicion && codigo[posicion] != '0') {
        posicion++;
        desc += _descomprimir(this->raiz, codigo.substr(posicion), posicion);
    }

    // std::cout << "palabra: " << desc << std::endl;
    // std::cout << "codigo: " << codigo << std::endl;
    // std::cout << "sobrante: " << codigo.substr(posicion) << std::endl;
    return desc;
}

char Arbol_huffman::_descomprimir(std::shared_ptr<nodo_arbol_huffman> nah,
                                  std::string codigo, int &posicion) {
    char respuesta = 0;

    if (codigo[0] == '1') {
        if (nah->hijo_i != NULL) {
            posicion++;
            respuesta = _descomprimir(nah->hijo_i, codigo.substr(1), posicion);
        } else {
            respuesta = nah->valor;
        }
    } else {
        if (nah->hijo_d != NULL) {
            posicion++;
            respuesta = _descomprimir(nah->hijo_d, codigo.substr(1), posicion);
        } else {
            respuesta = nah->valor;
        }
    }

    return respuesta;
}

void Arbol_huffman::imprimirPreOrden() {
    _imprimirPreOrden(std::shared_ptr<nodo_arbol_huffman>(this->raiz), 0);
}

void Arbol_huffman::_imprimirPreOrden(std::shared_ptr<nodo_arbol_huffman> nah,
                                      int depth) {

    for (int a = 0; a < depth; a++) {
        std::cout << "|  ";
    }
    nah->prt();

    if (nah->hijo_i != NULL)
        _imprimirPreOrden(nah->hijo_i, depth + 1);
    if (nah->hijo_d != NULL)
        _imprimirPreOrden(nah->hijo_d, depth + 1);
}

// No debe retornar una cadena que incluya '{'
std::string Arbol_huffman::comprimir(std::string cadena) {
    this->conseguirCodigos();
    std::string binString = "";
    std::string ret = "";

    for (char c : cadena) {
        binString += this->simbolos[c];
    }

    // encontrar la cantidad de bits faltantes
    // en caso de que binString.length() sea multiplo de 0
    // dejar el resultado final en 0
    int faltante = (8 - (binString.length() % 8)) % 8 ;

    for (int a = 0; a < faltante; a++) {
        binString += "0";
    }

    for (int a = 0; a < binString.length(); a += 8) {
        std::bitset<8> btc(binString.substr(a, a + 8));
        ret += (char)btc.to_ulong();
    }
    return ret;
}

std::string Arbol_huffman::toString() {
    if (this->raiz == nullptr )
        return "";
    return _toString(std::shared_ptr<nodo_arbol_huffman>(this->raiz));
}

std::string Arbol_huffman::_toString(std::shared_ptr<nodo_arbol_huffman> nah) {
    return "(" +
        nah->toString() +
        (nah->hijo_i != NULL ? _toString(nah->hijo_i) : "") +
        (nah->hijo_d != NULL ? _toString(nah->hijo_d) : "") +
        ")";
}

std::map<char, int> Arbol_huffman::conseguirTendencias() {
    return this->tendencias;
}

void Arbol_huffman::guardar(std::string nombre) {
    std::ofstream archivo(nombre);

    bool primer = true;

    for (std::pair<char, int> t : this->tendencias) {
        if (primer) {
            primer = false;
        } else {
            archivo << SEPARADOR;
            // archivo << std::endl;
        }
        archivo << t.first << ":" << t.second ;
    }

    archivo.close();
}

std::map<char, int> Arbol_huffman::cargar(std::string nombre) {
    std::map<char, int> tendencia;
    std::ifstream archivo(nombre);

    std::string cadena;
    char valor;

    // contador de caracteres SEPARADOR
    int contSep = 0;
    while (archivo.get(valor)) {
        cadena += valor;

        if (valor == SEPARADOR[0]) {
            contSep ++;

            // al encontrar 2, se sabe que es un SEPARADOR
            if (contSep == 2) {
                tendencia[cadena[0]] = std::stoi(cadena.substr(2));
                cadena = "";
                contSep = 0;
            }
        } else {
            contSep = 0;
        }
    }
    if (!cadena.empty())
    {
        tendencia[cadena[0]] = std::stoi(cadena.substr(2));
    }

    archivo.close();
    return tendencia;
}
