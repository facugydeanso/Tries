template <typename T>
string_map<T>::string_map(): raiz(nullptr){
    // COMPLETAR
}

template <typename T>
string_map<T>::string_map(const string_map<T>& aCopiar) : string_map() { *this = aCopiar; } // Provisto por la catedra: utiliza el operador asignacion para realizar la copia.

template <typename T>
string_map<T>& string_map<T>::operator=(const string_map<T>& d) {
    destruirAux(raiz);
    raiz = nullptr;
    Nodo* actual = d.raiz; //actual es el dicc a copiar
    if (actual == nullptr){
        return *this;
    }
    else{
        raiz = new Nodo(); //crea la copia
        Nodo* copia = raiz;
        if (actual->definicion != nullptr){
            raiz->definicion = new T(*(actual->definicion));
        }

        for (int i=0;i<256;i++){
            if ((actual->siguientes)[i] != nullptr){ //si la letra del diccionario a copiar esta definida entonces...
                    copiarAux(actual->siguientes[i], copia->siguientes[i]);
            }

        }
    }
}

template <typename T>
void string_map<T>::copiarAux(Nodo* actual, Nodo* &copia){
    copia = new Nodo(); //crea en la copia un nodo para esa letra
    if (actual->definicion != nullptr){
        copia->definicion = new T(*(actual->definicion)); //le da un significado si lo tiene
    }

    for (int i=0;i<256;i++){
        if ((actual->siguientes)[i] != nullptr){//
            copiarAux(actual->siguientes[i], copia->siguientes[i]);
        }
    }
}

template <typename T>
string_map<T>::~string_map(){
    destruirAux(raiz);
}

template <typename T>
T& string_map<T>::operator[](const string& clave){
    // COMPLETAR
}


template <typename T>
int string_map<T>::count(const string& clave) const{
    int res = 1;
    if (raiz == nullptr){
        res = 0;
    }
    else {
        Nodo* actual = raiz;
        for(int i = 0; i < clave.size();i++){
            if ((actual->siguientes)[clave[i]] != nullptr){
                actual = actual->siguientes[clave[i]];
                if (i == clave.size() - 1 && actual->definicion == nullptr){
                    res = 0;
                }
            }
            else{
                res = 0;
            }
        }
    }
    return res;
}

template <typename T>
const T& string_map<T>::at(const string& clave) const {
    Nodo* actual = raiz;
    string pal = clave;
    for(int i = 0; i < pal.size();i++){
        actual = actual->siguientes[pal[i]];
    }

    return *(actual->definicion);
}

template <typename T>
T& string_map<T>::at(const string& clave) {
    Nodo* actual = raiz;
    string pal = clave;
    for(int i = 0; i < pal.size();i++){
        actual = actual->siguientes[pal[i]];
    }
    return *(actual->definicion);
}

template <typename T>
void string_map<T>::erase(const string& clave) {
    Nodo* actual = raiz;
    string pal = clave;
    for(int i = 0; i < pal.size();i++){
        actual = actual->siguientes[pal[i]]; //va hacia la punta de la palabra
    }
    delete actual->definicion;
    actual->definicion = nullptr; //borra la definicion
    actual = actual->padre;
    int letra = pal.size() - 1;
    while((raiz == nullptr) && (actual == raiz) && (cantidadDeHijos(actual) == 1) && (letra >= 0)) {
        if (actual == raiz) {
            eraseAux(actual,pal,letra);
            delete raiz;
            raiz = nullptr;
        } else {
            eraseAux(actual, pal, letra); //le digo al padre que el hijo tiene que ser nulo
            letra--;
            actual = actual->padre;
        }
    }

}

template <typename T>
void string_map<T>::eraseAux(Nodo* pd, const string& clave, int i) {
    delete pd->siguientes[clave[i]];
    pd->siguientes[clave[i]] = nullptr;
}

template <typename T>
int string_map<T>::cantidadDeHijos(const Nodo *n) {
    int sum = 0;
    for (int i = 0; i < (n->siguientes).size(); i++){
        if (n->siguientes[i] != nullptr){
            sum++;
        }
    }
    return sum;
}

template <typename T>
int string_map<T>::size() const{
    int sum = 0;
    Nodo* actual = raiz;
    for(int i = 0; i < (actual->siguientes).size(); i++){
        if (actual->siguientes[i] != nullptr && (actual->siguientes[i])->definicion != nullptr){
            sum++;
        }
        if (actual->siguientes[i] != nullptr) {
            sum = sum + sizeAux(actual->siguientes[i]);
        }
    }
    return sum;
}

template <typename T>
int string_map<T>::sizeAux(Nodo* sig) const{
    int sum = 0;
    vector<Nodo*> sigs = sig->siguientes;
    for(int i = 0; i < (sigs).size(); i++){
        if (sigs[i] != nullptr && (sigs[i])->definicion != nullptr){
            sum++;
        }
        else{
            if (sigs[i] != nullptr) {
                sum = sum + sizeAux(sigs[i]);
            }
        }
    }
    return sum;
}

template <typename T>
bool string_map<T>::empty() const{
    bool res = true;
    if (raiz == nullptr){
        res = true;
    }
    else {
        vector<Nodo*> ps = raiz->siguientes;
        for (int i = 0; i < 256; i++) {
            if (ps[i] != nullptr) {
                res = false;
            }
        }
    }
    return res;
}

template <typename T>
void string_map<T>::insert(const pair<string,T>& value_type){
    if (raiz == nullptr){
        string pal = value_type.first;
        T* def = new T(value_type.second);
        raiz = new Nodo();
        Nodo* actual = raiz;
        for(int i = 0; i < pal.size();i++){
            (actual->siguientes)[pal[i]] = new Nodo();
            (actual->siguientes[pal[i]])->padre = actual;
            actual = (actual->siguientes)[pal[i]];
            if (i == pal.size() - 1){
                actual->definicion = def;
            }
        }
    }
    else {
        Nodo* actual = raiz;
        string pal = value_type.first;
        T* def = new T(value_type.second);
        for(int i = 0; i < pal.size();i++){
            if (actual->siguientes[pal[i]] != nullptr){
                actual = actual->siguientes[pal[i]];
            }
            else{
                (actual->siguientes)[pal[i]] = new Nodo();
                (actual->siguientes[pal[i]])->padre = actual;
                actual = (actual->siguientes)[pal[i]];
            }
            if (i == pal.size() - 1){
                delete actual->definicion;
                actual->definicion = def;
            }
        }
    }

}

template <typename T>
void string_map<T>::destruirAux(Nodo *n) const{
    if (n != nullptr){
        for(int i = 0; i<256;i++){
            destruirAux((n->siguientes)[i]);
        }
        if (n->definicion != nullptr){
            delete n->definicion;
        }
        delete n;
    }
}