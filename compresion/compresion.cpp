
#include "compresion.hpp"

/*
Begin
   definir un nodo con caracter, frecuencia, e hijo izquierdo y derecho.
   create a list ‘freq’ to store frequency of each character, initially, all are 0
   for each character c in the string do
      increase the frequency for character ch in freq list.
   done

   for all type of character ch do
      if the frequency of ch is non zero then
         add ch and its frequency as a node of priority queue Q.
   done

   while Q is not empty do
      remove item from Q and assign it to left child of node
      remove item from Q and assign to the right child of node
      traverse the node to find the assigned code
   done
End
*/

void sumarMapas(std::map<char, int> &map1, std::map<char, int> map2) {
    for (std::pair<char, int> p: map2) {
        // si ya existe lo suma, de lo contrario lo crea
        map1[p.first] ? map1[p.first] += p.second : map1[p.first] = p.second ;
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

    nodo_arbol_huffman nah = cp.top();
    cp.pop();

    this->raiz = std::make_shared<nodo_arbol_huffman>(nah);
}

void Arbol_huffman::conseguirCodigos() {
    _conseguirCodigos(this->raiz, "1");

    // for (std::pair<char, std::string> v : this->simbolos) {
    // 	std::cout << v.first << "->" << v.second << std::endl;
    // }
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
    for (char c : binCodigo) {
        std::bitset<8> btc(c);
        codigo += btc.to_string();
    }
    std::string desc = "";
    int posicion = 0;
    while (codigo.length() > posicion && codigo[posicion] != '0') {
        posicion++;
        desc += _descomprimir(this->raiz, codigo.substr(posicion), posicion);
    }

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

std::string Arbol_huffman::comprimir(std::string cadena) {
    this->conseguirCodigos();
    std::string binString = "";
    std::string ret = "";

    for (char c : cadena) {
        binString += this->simbolos[c];
    }

    int faltante = binString.length() % 8;

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

    // TODO: hay que buscar alguna forma de manejar EOL
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

std::map<char, int> stringATendencia(std::string cadena) {
    std::map<char, int> tendencias;
    // std::cout << cadena << std::endl;
    std::size_t found = cadena.find(SEPARADOR);

    int rep = 0;

    // longitud del separador
    int l_sep = sizeof(SEPARADOR)/sizeof(SEPARADOR[0]);

    while (found!=std::string::npos && rep < 50) {
        std::string pal = cadena.substr(0, found);
        // std::cout << pal << " -- " << pal.size() << std::endl;

        tendencias[pal[0]] = std::stoi(pal.substr(2));

        cadena = cadena.substr(found + l_sep - 1);

        found = cadena.find(SEPARADOR);
        rep ++;
    }

    return tendencias;
}

std::map<char, int> Arbol_huffman::cargar(std::string nombre) {
    std::map<char, int> tendencia;
    std::ifstream archivo(nombre);

    bool primer = true;
    std::string texto;
    while (getline (archivo, texto )) {
        sumarMapas(tendencia, stringATendencia(texto));
        // Output the text from the file
        // for (std::pair<char, int> t : stringATendencia(texto)) {
        //     std::cout << t.first << " -> " << t.second <<std::endl;
        // }
    }

    archivo.close();
    return tendencia;
}
