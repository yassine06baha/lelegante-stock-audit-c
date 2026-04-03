const stockData = window.LELEGANTE_STOCK_REPORT || [];
const categoryFilter = document.getElementById("categoryFilter");
const stockGrid = document.getElementById("stockGrid");
const budgetValue = document.getElementById("budgetValue");

function buildCategoryOptions() {
    const categories = [...new Set(stockData.map((product) => product.category))].sort();

    categoryFilter.innerHTML += categories
        .map((category) => `<option value="${category}">${category}</option>`)
        .join("");
}

function renderProducts() {
    const selectedCategory = categoryFilter.value;
    const filteredProducts =
        selectedCategory === "all"
            ? stockData
            : stockData.filter((product) => product.category === selectedCategory);

    const totalBudget = filteredProducts.reduce(
        (sum, product) => sum + product.estimatedBudget,
        0
    );

    budgetValue.textContent = `${totalBudget.toFixed(2)} EUR`;

    stockGrid.innerHTML = filteredProducts
        .map(
            (product) => `
                <article class="stock-card">
                    <p class="category">${product.category}</p>
                    <h2>${product.name}</h2>
                    <div class="score">
                        Score priorité
                        <strong>${product.priorityScore}</strong>
                    </div>
                    <div class="stock-line">
                        <span>Stock actuel</span>
                        <strong>${product.stockQuantity}</strong>
                    </div>
                    <div class="stock-line">
                        <span>Seuil mini</span>
                        <strong>${product.reorderLevel}</strong>
                    </div>
                    <div class="stock-line">
                        <span>Ventes mensuelles</span>
                        <strong>${product.monthlySales}</strong>
                    </div>
                    <div class="stock-line">
                        <span>Budget estimé</span>
                        <strong>${product.estimatedBudget.toFixed(2)} EUR</strong>
                    </div>
                </article>
            `
        )
        .join("");
}

buildCategoryOptions();
categoryFilter.addEventListener("change", renderProducts);
renderProducts();
