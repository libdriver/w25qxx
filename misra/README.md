<div align=center>
<img src="/doc/image/misra.png"/>
</div>


### Table of Contents

  - [Introduce](#Introduce)
  - [Classification](#Classification)
  - [Compliance](#Compliance)
  - [Code](#Code)
  - [Report](#Report)

### Introduce

MISRA C is a set of software development guidelines for the C programming language developed by The MISRA Consortium. Its aims are to facilitate code safety, security, portability and reliability in the context of embedded systems, specifically those systems programmed in ISO C / C90 / C99.

Although originally specifically targeted at the automotive industry, MISRA C has evolved as a widely accepted model for best practices by leading developers in sectors including automotive, aerospace, telecom, medical devices, defense, railway, and others.

### Classification

Each Guideline is classified as Mandatory(new for MISRA C:2012), Required or Advisory.

- Mandatory guidelines shall always be complied with
- Required guidelines shall be complied with, unless subject to a Deviation
- Advisory guidelines are considered good practice, but compliance is less formal.

### Compliance

In order for a piece of software to claim to be compliant to the MISRA C Guidelines, all mandatory rules shall be met and all required rules and directives shall either be met or subject to a formal deviation.

Many MISRA C rules can be characterized as guidelines because under certain condition software engineers may deviate from rules and still be considered compliant with the standard. Deviations must be documented either in the code or in a file. In addition; proof must be provided that the software engineer has considered the safety of the system and that deviating from the rule will not have a negative impact, requirements for deviations also include:

- The rule deviated from.
- Rationale for deviation.

### Code

LibDriver complies with deviated MISRA standards.Libdriver complies with all mandatory guidelines and most required guidelines, the deviations are shown in Table 1.

<div align=center> Table 1. Guideline Deviations </div>

| Guideline | Description                                                  | Status       | Explanation                                                  | Severity |
| --------- | ------------------------------------------------------------ | ------------ | ------------------------------------------------------------ | -------- |
| 10.1      | Operands shall not be of an inappropriate essential type. (\|, &, ~, <<, >>) | incompatible | Embedded drivers need this method to set or clear some bits and drivers guarantee the safety of the operation. | very low |
| 10.3      | The value of an expression shall not be assigned to an object with a narrower essential type or of a different essential type category.  (<<, >>) | incompatible | Embedded drivers need this method to set or clear some bits and drivers guarantee the safety of the operation. | very low |
| 10.4      | Both operands of an operator in which the usual arithmetic conversions are performed shall have the same essential type category.  (\|, &, <<, >>) | incompatible | We use enumeration to define driver configuration, which is a friendly programming method and should be accepted and drivers guarantee the safety of the operation. | very low |
| 10.8      | The value of a composite expression shall not be cast to a different essential type category or a wider essential type. | incompatible | Embedded drivers need this method to set or clear some bits and drivers guarantee the safety of the operation. | very low |

### Report

Detailed reports can be found in /misra/Polyspace report.pdf.
