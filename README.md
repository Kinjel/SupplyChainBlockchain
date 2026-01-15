# Supply Chain Blockchain (Qt Project)

A desktop-based **Blockchain Supply Chain Tracker** built using **Qt (C++)**.  
This project demonstrates how blockchain concepts can be applied to supply chain data using a graphical user interface.

---

## Features

- Add supply chain batches (Batch ID, Material, Quantity, Timestamp)
- Automatically generate hashes for each batch
- Blockchain-style **chain verification**
- Detects tampering in previous records
- Visual representation using **bar chart**
- Delete selected batch
- Export-ready table structure
- Clean and user-friendly Qt GUI

---

## Technologies Used

- **C++**
- **Qt 6**
- Qt Widgets
- Qt Charts
- Git & GitHub

---

## Application Overview

- Top section shows a **bar chart** of quantities per batch
- Middle section contains control buttons:
  - Add Batch
  - Delete Batch
  - Verify Chain
  - Export
- Bottom section displays batch data in a table

---

## Blockchain Logic (Simple Explanation)

- Each batch stores a hash generated from its data
- Every new batch depends on the previous batch
- If any record is modified or deleted, **Verify Chain** will detect the issue
- Ensures data integrity similar to a blockchain

---

## How to Run the Project

1. Install **Qt 6 (with MinGW)**  
2. Open **Qt Creator**
3. Click **File → Open File or Project**
4. Select `SupplyChainBlockchain.pro`
5. Configure kit (Desktop Qt 6 MinGW)
6. Build & Run

---

## 📂 Project Structure

