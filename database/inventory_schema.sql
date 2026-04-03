CREATE TABLE products (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    category TEXT NOT NULL,
    stock_quantity INTEGER NOT NULL CHECK (stock_quantity >= 0),
    reorder_level INTEGER NOT NULL CHECK (reorder_level >= 0),
    monthly_sales INTEGER NOT NULL CHECK (monthly_sales >= 0),
    unit_cost REAL NOT NULL CHECK (unit_cost > 0)
);

CREATE VIEW critical_stock AS
SELECT
    id,
    name,
    category,
    stock_quantity,
    reorder_level,
    monthly_sales,
    unit_cost,
    (reorder_level - stock_quantity) * monthly_sales AS priority_score
FROM products
WHERE stock_quantity <= reorder_level
ORDER BY priority_score DESC;
