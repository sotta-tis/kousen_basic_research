// 寿司のタイプを引数として受け取り、モーダルウィンドウを開く関数
function openModal(sushiType) {
    document.getElementById('orderModal').style.display = 'block'; // モーダルを表示
    document.getElementById('modalTitle').innerHTML = sushiType; // モーダルのタイトルを設定
    document.getElementById('modalImage').src = `/public/src/${sushiType.toLowerCase()}.png`; // 対応する寿司の画像を設定
    resetQuantityButtons(); // 皿数選択ボタンの選択状態をリセット
    defaultQuantity(); // 最初は1皿を選択しとく

}

// 皿数を選択するための関数
function selectQuantity(quantity) {
    resetQuantityButtons(); // 一旦全ての選択状態を解除
    document.querySelector(`.quantity-buttons button[data-quantity="${quantity}"]`).classList.add('selected'); // 選択した皿数のボタンに'selected'クラスを追加してスタイルを適用
}

// すべての皿数選択ボタンの選択状態をリセットする関数
function resetQuantityButtons() {
    document.querySelectorAll('.quantity-buttons button').forEach(button => {
        button.classList.remove('selected'); // 各ボタンから'selected'クラスを削除
    });
}

function defaultQuantity() {
    resetQuantityButtons(); // すべての選択状態をリセット
    document.querySelector('.quantity-buttons button[data-quantity="1"]').classList.add('selected'); // 1皿のボタンに選択状態を適用
}

// モーダルウィンドウを閉じるためのイベントハンドラ
document.querySelector('.close').onclick = function() {
    document.getElementById('orderModal').style.display = 'none'; // モーダルウィンドウを非表示に
}

// 最終的な注文を処理するためのイベントハンドラ
document.querySelector('.final-order-button').onclick = function() {
    var selectedButton = document.querySelector('.quantity-buttons button.selected'); // 選択された皿数のボタンを取得
    var quantity = selectedButton ? selectedButton.getAttribute('data-quantity') : 1; // 選択された皿数を取得、選択がなければ0を設定
    window.location.href = `/order?item=${document.getElementById('modalTitle').textContent}&quantity=${quantity}`; // 注文ページへリダイレクトし、アイテム名と数量をクエリパラメータとして渡す
}
