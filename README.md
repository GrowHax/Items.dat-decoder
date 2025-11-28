# Archive of decoded items.dat

## Information

The `items.dat` file format has evolved through various versions, with changes in data structure and additional fields being introduced over time. Below is a summary of the modifications made to the data structure for each version:

### `version 11`
- **New Field:** `punch_option`
  - **Description:** A new string field named `punch_option` was introduced.

### `version 12`
- **Data Skipping:** 
  - **Description:** An additional 13 bytes of data are skipped in this version.

### `version 13`
- **Data Skipping:** 
  - **Description:** An additional 4 bytes of data are skipped in this version.

### `version 14`
- **Data Skipping:** 
  - **Description:** An additional 4 bytes of data are skipped in this version.

### `version 15`
- **Data Handling:**
  - **Description:** An additional 25 bytes of data are skipped, followed by reading a new string.

### `version 16`
- **Data Handling:**
  - **Description:** A new string is read in this version.

### `version 17`
- **Data Skipping:**
  - **Description:** An additional 4 bytes of data are skipped in this version.

### `version 18`
- **Data Skipping:**
  - **Description:** An additional 4 bytes of data are skipped in this version.
