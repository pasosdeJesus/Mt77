Mt77 es un motor de búsqueda rápido, preciso, apto para indexación
distribuida y de dominio público

1. Características:
  - Más veloz en búsquedas que Lucene y que Amberfish, aunque los
    índices son más grandes y requieren más tiempo de construcción
  - Especialmente diseñado para textos en Español
  - Puede leer textos planos, archivos XML, HTML, PDF y ODF
  - Permite buscar una o varias palabras, así como  una o más cadenas
  - Permite indexar y buscar metainformación (título, tipo de archivo,
    sitio de origen, etiquetas XML)
  - Escrito en C++ siguiendo estándares, con pruebas de unidad
    (con make limpia; make prueba; make unidad) y de regresión (con make limpia; make prueba; make regr)

2. Herramientas que produce

Consta de las siguientes herramientas:

- __buscador__ para buscar palabras o metainformación en un índice
- __indexador__ para crear un índice a partir de uno o más textos
- __operaindice__ para mezclar índices en disco o RAM y hacer otras operaciones
- __txtdeodt__ extraer texto de un documento odt (Open Document Format)
- __unzipuno__ extraer un sólo archivo de un conteneder ZIP
- __tomsha256__ Calcula hash sha256 empleando rutina de libtomcrypt-1.16

3. Herramientas para el desarrollo y estándares

- `make doctec` Genera documentación de fuentes
- `make unidad` Ejecuta pruebas de unidad. Pero sólo debe hacerse después de 
  compilar con aserciones `make limpia prueba`
- `make regr` Ejecuta pruebas de regresión (deben pasar tanto con como sin
    aserciones).
- `make cobertura` Verificar cobertura de las pruebas de unidad
- `make desempeno` Análisis de desempeño

