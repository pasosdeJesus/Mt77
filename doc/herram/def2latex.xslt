<?xml version="1.0" encoding="ISO-8859-1"?>
<!-- vim: set expandtab tabstop=4 shiftwidth=4 foldmethod=marker: -->
<!-- A partir de un archivo .def genera macros en LaTeX
Se cede esta fuente al dominio público 2004.  Sin garantías.
http://structio.sourceforge.net/sigue
structio-info@lists.sourceforge.net -->
<!DOCTYPE xsl:stylesheet [
]>

<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

    <xsl:output method="text" encoding="ISO-8859-1" />

    <xsl:param name="filename"/>

    <xsl:template match="def">
        <xsl:apply-templates select="pal"/>
    </xsl:template>

    <xsl:template match="pal">
    <xsl:choose>
        <xsl:when test="./@pal!=''"> 
            <xsl:text>\pal{</xsl:text>
            <xsl:value-of select="@pal"/>
            <xsl:text>}{log.ind.</xsl:text>
            <xsl:value-of select="@id"/>
            <xsl:text>}</xsl:text>
        </xsl:when>
        <xsl:otherwise>
            <xsl:text>\pal{</xsl:text>
            <xsl:value-of select="@id"/>
            <xsl:text>}{log.ind}</xsl:text>
        </xsl:otherwise>
    </xsl:choose>
    <xsl:text>{0}{</xsl:text>
    <xsl:apply-templates select="sig"/>
    <xsl:text>}{</xsl:text>
    <xsl:apply-templates select="pista"/>
    <xsl:text>}{</xsl:text>
    <xsl:apply-templates select="despiste"/>
    <xsl:text>}
    </xsl:text>
</xsl:template>

<xsl:template match="sig">
    <xsl:value-of select="."/>
</xsl:template>

<xsl:template match="pista[1]">
    <xsl:value-of select="."/>
</xsl:template>

<xsl:template match="pista">
    <xsl:text>|</xsl:text>
    <xsl:value-of select="."/>
</xsl:template>

<xsl:template match="despiste[1]">
    <xsl:value-of select="."/>
</xsl:template>

<xsl:template match="despiste">
    <xsl:text>|</xsl:text>
    <xsl:value-of select="."/>
</xsl:template>

</xsl:stylesheet>


