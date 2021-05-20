Mt77 es un motor de búsqueda rápido, preciso, apto para indexación
distribuida y con licencia ISC

# 1. Características:
  - Más veloz en búsquedas que Lucene y que Amberfish, aunque los
    índices son más grandes y requieren más tiempo de construcción
  - Especialmente diseñado para textos en Español
  - Puede leer en codificación UTF-8 textos planos, archivos XML, HTML, PDF, 
    y ODF.  Con la opción -l el indexador también puede leer textos planos en 
    codificación LATIN1.
  - Permite buscar una o varias palabras, así como  una o más cadenas. 
    Se especifican en UTF-8 aunque opcionalmente (empleando opción `-l`
    del buscador) pueden especificarse en LATIN1.
  - Permite indexar y buscar metainformación (título, tipo de archivo,
    sitio de origen, etiquetas XML)
  - Escrito en C++ siguiendo estándares, con pruebas de unidad
    (con `make limpia; make prueba; make unidad`) y de regresión 
    (con `make limpia; make prueba; make regr`)


# 2. Ejemplo de compilación y uso con textos planos

Primero compile con:
```
    ./conf.sh
    make
```

Que entre otras generará las herramientas `indexador` y `buscador` para
crear un índice y para buscar sobre un índice.

Con las fuentes se incluyen textos de prueba en el directorio `regr` como
por ejemplo el evangelio de Mateo como texto plano en codificación UTF-8 
`regr/mateo-utf8.txt`.

Para crear un índice `m.indice` (con relación de documentos indexados
`m.relacion`) con las palabras de `regr/mateo-utf8.txt` referenciando el 
texto con el URL <https://traduccion.pasosdeJesus.org>:
```
   rm -f m.indice m.relacion t.indice;
   ./indexador m.indice t.indice 'https://traduccion.pasosdeJesus.org' regr/mateo-utf8.txt
```

Para búscar la palabra VERDAD en el índice creado:
```
   ./buscador m.indice VERDAD
```

Que retornará un JSON indicando que encontró 14 ocurrencias de la palabra:
```
{
  "consulta": "VERDAD",
  "documentos": 1,
  "inicio": 1,
  "fin": 1,
  "resultados": [
    { "url": "https://traduccion.pasosdeJesus.orgregr/mateo-utf8.txt",
    "frec": "14",
    "fecha": "2020-08-17"
    }
  ]
}
```

Para agregar al mismo índice `m.indice` (con relación `m.relacion`) las 
palabras de `regr/marcos-utf8.txt`:
```
   ./indexador m.indice t.indice 'https://traduccion.pasosdeJesus.org' regr/marcos-utf8.txt
```

Y al repetir la misma búsqueda  ya se obtendrán  1 resultado en Marcos y 
los 14 de Mateo:
```
$ ./buscador m.indice VERDAD                                             
{
  "consulta": "VERDAD",
  "documentos": 2,
  "inicio": 1,
  "fin": 2,
  "resultados": [
    { "url": "https://traduccion.pasosdeJesus.orgregr/marcos-utf8.txt",
    "frec": "1",
    "fecha": "2020-08-18"
    }, 
    { "url": "https://traduccion.pasosdeJesus.orgregr/mateo-utf8.txt",
    "frec": "14",
    "fecha": "2020-08-17"
    }
  ]
}
``` 

# 3. Herramientas que produce al compilar

Consta de las siguientes herramientas:

- __buscador__ para buscar palabras o metainformación en un índice
- __depueraindice__ para examinar un índice interactivamente
- __indexador__ para crear un índice a partir de uno o más textos
- __operaindice__ para mezclar índices en disco o RAM y hacer otras operaciones
- __txtdeodt__ extraer texto de un documento odt (Open Document Format)
- __unzipuno__ extraer un sólo archivo de un conteneder ZIP
- __tomsha256__ Calcula hash sha256 empleando rutina de libtomcrypt-1.16

# 4. Herramientas para el desarrollo y estándares

- `make all` Compila herramientas sin aserciones y con optimizaciones
- `make prueba` Compila pero activando aserciones y sin optimizaciones
- `make limpia` Limpia ejecutables generados al compilar
- `make unidad` Ejecuta pruebas de unidad. Pero sólo debe hacerse después de 
  compilar con aserciones `make limpia prueba`
- `make regr` Ejecuta pruebas de regresión (deben pasar tanto con como sin
    aserciones).
- `make cobertura` Verificar cobertura de las pruebas de unidad
- `make desempeno` Análisis de desempeño
- `make doctec` Genera documentación de fuentes

