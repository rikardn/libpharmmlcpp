<!--
Copyright 2016 Rikard Nordgren

Licensed under the Apache License, Version 2.0 (the "License"); you
may not use this file except in compliance with the License.  You may
obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0
     
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
implied. See the License for the specific language governing
permissions and limitations under the License.
-->

<!-- The transformation is from PharmML 0.8.1 to 0.9 -->
<xsl:stylesheet version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:pharmml="http://www.pharmml.org/pharmml/0.8/PharmML"
	xmlns:ct="http://www.pharmml.org/pharmml/0.8/CommonTypes"
	xmlns:math="http://www.pharmml.org/pharmml/0.8/Maths"
	xmlns:ds="http://www.pharmml.org/pharmml/0.8/Dataset"
	xmlns:mstep="http://www.pharmml.org/pharmml/0.8/ModellingSteps"
	xmlns:mdef="http://www.pharmml.org/pharmml/0.8/ModelDefinition"
	xmlns:design="http://www.pharmml.org/pharmml/0.8/TrialDesign"
    xmlns:po="http://www.pharmml.org/probonto/ProbOnto"
    exclude-result-prefixes="pharmml ct math ds mstep mdef design po">
    <xsl:output method="xml" indent="yes"/>

	<!-- identity transform -->
    <xsl:template match="node()|@*" name="identity">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
        </xsl:copy>
    </xsl:template>

	<!-- New namespaces -->
	<xsl:template match="pharmml:*">
		<xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.9/PharmML">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="ct:*">
		<xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.9/CommonTypes">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="math:*">
		<xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.9/Maths">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="ds:*">
		<xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.9/Dataset">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="mdef:*">
		<xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.9/ModelDefinition">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="mstep:*">
		<xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.9/ModellingSteps">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="design:*">
		<xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.9/TrialDesign">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
	</xsl:template>

	<xsl:template match="pharmml:PharmML/@writtenVersion">
		<xsl:attribute name="writtenVersion">
			<xsl:value-of select="'0.9'"/>
		</xsl:attribute>
	</xsl:template>

    <!-- Move all ConditionalStatement into math namespace -->
    <xsl:template match="//*[local-name()='ConditionalStatement']">
		<xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.9/Maths">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
	</xsl:template>

    <!-- FixedEffect Category into CatRef -->
    <xsl:template match="mdef:FixedEffect/mdef:Category/@catId">
		 <xsl:attribute name="catIdRef">
			 <xsl:value-of select="."/>
		 </xsl:attribute>
	 </xsl:template>
    <xsl:template match="mdef:FixedEffect/mdef:Category">
        <xsl:element name="CatRef" namespace="http://www.pharmml.org/pharmml/0.9/CommonTypes">
			<xsl:apply-templates select="@*|node()"/>
        </xsl:element>
    </xsl:template>

    <!-- Wrap IndividualParameter//FixedEffect in Assign -->
    <xsl:template match="mdef:IndividualParameter//mdef:FixedEffect">
        <xsl:element name="FixedEffect" namespace="http://www.pharmml.org/pharmml/0.9/ModelDefinition">
            <xsl:apply-templates select="@*"/>
            <xsl:element name="Assign" namespace="http://www.pharmml.org/pharmml/0.9/CommonTypes">
                <xsl:apply-templates select="node()[not(self::mdef:Category)]"/>
            </xsl:element>
            <xsl:apply-templates select="mdef:Category"/>
        </xsl:element>
    </xsl:template>

    <!-- Wrap IndividualParameter//RandomEffect in Assign -->
    <xsl:template match="mdef:IndividualParameter//mdef:RandomEffects">
        <xsl:element name="RandomEffects" namespace="http://www.pharmml.org/pharmml/0.9/ModelDefinition">
            <xsl:apply-templates select="@*"/>
            <xsl:element name="Assign" namespace="http://www.pharmml.org/pharmml/0.9/CommonTypes">
                <xsl:apply-templates select="node()"/>
            </xsl:element>
        </xsl:element>
    </xsl:template>

    <!-- Wrap Correlation//RandomVariable1 in Assign -->
    <xsl:template match="mdef:Correlation//mdef:RandomVariable1">
        <xsl:element name="RandomVariable1" namespace="http://www.pharmml.org/pharmml/0.9/ModelDefinition">
            <xsl:apply-templates select="@*"/>
            <xsl:element name="Assign" namespace="http://www.pharmml.org/pharmml/0.9/CommonTypes">
                <xsl:apply-templates select="node()"/>
            </xsl:element>
        </xsl:element>
    </xsl:template>

    <!-- Wrap Correlation//RandomVariable2 in Assign -->
    <xsl:template match="mdef:Correlation//mdef:RandomVariable2">
        <xsl:element name="RandomVariable2" namespace="http://www.pharmml.org/pharmml/0.9/ModelDefinition">
            <xsl:apply-templates select="@*"/>
            <xsl:element name="Assign" namespace="http://www.pharmml.org/pharmml/0.9/CommonTypes">
                <xsl:apply-templates select="node()"/>
            </xsl:element>
        </xsl:element>
    </xsl:template>


    <!-- Put oid on all DesignSpace and IndividualAdministration. Use DesignSpace1 etc without checking for collisions -->
    <xsl:template match="design:DesignSpace|design:IndividualAdministration">
        <xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.9/TrialDesign">
            <xsl:variable name="index">
                <xsl:number level="any"/>
            </xsl:variable>
            <xsl:attribute name="oid">
                <xsl:value-of select="local-name()"/>
                <xsl:value-of select="$index"/>
            </xsl:attribute>
			<xsl:apply-templates select="@*|node()"/>
        </xsl:element>
    </xsl:template>

</xsl:stylesheet>
