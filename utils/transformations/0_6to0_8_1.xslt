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

<!-- This transformation is from PharmML 0.6 to PharmML 0.8.1 -->
<xsl:stylesheet version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:pharmml="http://www.pharmml.org/pharmml/0.6/PharmML"
	xmlns:ct="http://www.pharmml.org/pharmml/0.6/CommonTypes"
    xmlns:oldct="http://www.pharmml.org/2013/03/CommonTypes"
	xmlns:math="http://www.pharmml.org/pharmml/0.6/Maths"
	xmlns:ds="http://www.pharmml.org/pharmml/0.6/Dataset"
	xmlns:mstep="http://www.pharmml.org/pharmml/0.6/ModellingSteps"
	xmlns:mdef="http://www.pharmml.org/pharmml/0.6/ModelDefinition"
	xmlns:design="http://www.pharmml.org/pharmml/0.6/TrialDesign"
    xmlns:uncert="http://www.uncertml.org/3.0"
    xmlns:po="http://www.pharmml.org/probonto/ProbOnto"
    exclude-result-prefixes="pharmml oldct ct math ds mstep mdef design uncert po">
    <xsl:output method="xml" indent="yes"/>

	<!-- identity transform -->
    <xsl:template match="node()|@*" name="identity">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
        </xsl:copy>
    </xsl:template>

	<!-- New namespaces -->
	<xsl:template match="pharmml:*">
		<xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.8/PharmML">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="ct:*|oldct:*">
		<xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.8/CommonTypes">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="math:*">
		<xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.8/Maths">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="ds:*">
		<xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.8/Dataset">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="mdef:*">
		<xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.8/ModelDefinition">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="mstep:*">
		<xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.8/ModellingSteps">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="design:*">
		<xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.8/TrialDesign">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
	</xsl:template>

	<xsl:template match="pharmml:PharmML/@writtenVersion">
		<xsl:attribute name="writtenVersion">
			<xsl:value-of select="'0.8.1'"/>
		</xsl:attribute>
	</xsl:template>

    <!-- New namespace for Piecewise -->
	<xsl:template match="ds:Piecewise">
		<xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.8/Maths">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
	</xsl:template>
    <xsl:template match="mstep:MultipleDVMapping/mstep:Piecewise">
		<xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.8/TrialDesign">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
    </xsl:template>

	<!-- SimpleParameter -> Parameter -->
	<xsl:template match="mdef:SimpleParameter">
		<xsl:element name="PopulationParameter" namespace="http://www.pharmml.org/pharmml/0.8/ModelDefinition">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
	</xsl:template>

    <!-- IndividualParameter -->
    <xsl:template match="mdef:GaussianModel">
		<xsl:element name="StructuredModel" namespace="http://www.pharmml.org/pharmml/0.8/ModelDefinition">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
    </xsl:template>
    <xsl:template match="mdef:Transformation[text()='log']">
        <Transformation xmlns="http://www.pharmml.org/pharmml/0.8/ModelDefinition" type='log'/>
    </xsl:template>
    <xsl:template match="mdef:LinearCovariate/mdef:PopulationParameter">
        <PopulationValue xmlns="http://www.pharmml.org/pharmml/0.8/ModelDefinition">
			<xsl:apply-templates select="@*|node()"/>
        </PopulationValue>
    </xsl:template>

	<!-- Remove Equation after certain elements -->
	<xsl:template match="ct:Assign/math:Equation">
		<xsl:apply-templates/>
	</xsl:template>
    <xsl:template match="mstep:InitialEstimate/math:Equation">
        <xsl:apply-templates/>
    </xsl:template>
    <xsl:template match="mstep:LowerBound/math:Equation">
        <xsl:apply-templates/>
    </xsl:template>
    <xsl:template match="mstep:UpperBound/math:Equation">
        <xsl:apply-templates/>
    </xsl:template>

	<!-- Replace all other Equation with Assign -->
	<xsl:template match="math:Equation">
	    <xsl:element name="Assign" namespace="http://www.pharmml.org/pharmml/0.8/CommonTypes">
      		<xsl:apply-templates select="@*|node()"/>
    	</xsl:element>
	</xsl:template>

	<!-- UncertML to Probonto -->
    <xsl:template match="uncert:NormalDistribution[uncert:variance]">
        <Distribution xmlns="http://www.pharmml.org/pharmml/0.8/ModelDefinition">
            <ProbOnto xmlns="http://www.pharmml.org/probonto/ProbOnto" name="Normal2">
                <xsl:apply-templates/>
            </ProbOnto>
        </Distribution>
	</xsl:template>

    <xsl:template match="uncert:NormalDistribution[uncert:stddev]">
        <Distribution xmlns="http://www.pharmml.org/pharmml/0.8/ModelDefinition">
            <ProbOnto xmlns="http://www.pharmml.org/probonto/ProbOnto" name="Normal1">
                <xsl:apply-templates/>
            </ProbOnto>
        </Distribution>
	</xsl:template>

    <xsl:template match="uncert:mean">
        <Parameter name="mean" xmlns="http://www.pharmml.org/probonto/ProbOnto">
            <Assign xmlns="http://www.pharmml.org/pharmml/0.8/CommonTypes">
                <xsl:apply-templates select="@*|node()"/>
            </Assign>
        </Parameter>
    </xsl:template>

    <xsl:template match="uncert:variance">
        <Parameter name="var" xmlns="http://www.pharmml.org/probonto/ProbOnto">
            <Assign xmlns="http://www.pharmml.org/pharmml/0.8/CommonTypes">
                <xsl:apply-templates select="@*|node()"/>
            </Assign>
        </Parameter>
    </xsl:template>

    <xsl:template match="uncert:stddev">
        <Parameter name="stdev" xmlns="http://www.pharmml.org/probonto/ProbOnto">
            <Assign xmlns="http://www.pharmml.org/pharmml/0.8/CommonTypes">
                <xsl:apply-templates select="@*|node()"/>
            </Assign>
        </Parameter>
    </xsl:template>

    <xsl:template match="uncert:rVal|uncert:prVal">
        <Real xmlns="http://www.pharmml.org/pharmml/0.8/CommonTypes">
            <xsl:value-of select="."/>
        </Real>
    </xsl:template>

    <xsl:template match="uncert:var">
        <SymbRef xmlns="http://www.pharmml.org/pharmml/0.8/CommonTypes">
            <xsl:attribute name="symbIdRef">
                <xsl:value-of select="./@varId"/>
            </xsl:attribute>
        </SymbRef>
    </xsl:template>

    <!-- Move ExternalDataSet to TrialDesign -->
	<xsl:template match="mstep:ExternalDataSet">
		<xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.8/TrialDesign">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
	</xsl:template>
    <xsl:template match="mstep:ColumnMapping">
		<xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.8/TrialDesign">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
    </xsl:template>
    <xsl:template match="mstep:MultipleDVMapping">
		<xsl:element name="{local-name()}" namespace="http://www.pharmml.org/pharmml/0.8/TrialDesign">
			<xsl:apply-templates select="@*|node()"/>
		</xsl:element>
    </xsl:template>
    <xsl:template match="mstep:ModellingSteps">
		<xsl:element name="TrialDesign" namespace="http://www.pharmml.org/pharmml/0.8/TrialDesign">
            <xsl:apply-templates select="mstep:ExternalDataSet"/>
		</xsl:element>
		<xsl:element name="ModellingSteps" namespace="http://www.pharmml.org/pharmml/0.8/ModellingSteps">
            <xsl:apply-templates select="@*|node()[not(self::mstep:ExternalDataSet)]"/>
        </xsl:element>
    </xsl:template>

</xsl:stylesheet>
